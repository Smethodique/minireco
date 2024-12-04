/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iabboudi <iabboudi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 22:19:33 by iabboudi          #+#    #+#             */
/*   Updated: 2024/11/16 22:59:19 by iabboudi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	setup_redirection(t_command *cmd)
{
	int	in;
	int	out;

	in = get_in(cmd->redirections, STDIN_FILENO);
	out = get_out(cmd, STDOUT_FILENO);
	if (out == -1 || in == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->redirections->filename, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		exit(1);
	}
	if (in != STDIN_FILENO)
	{
		dup2(in, STDIN_FILENO);
		close(in);
	}
	if (out != STDOUT_FILENO)
	{
		dup2(out, STDOUT_FILENO);
		close(out);
	}
}

void	redic_not_builtin(t_command *cmd, char **env)
{
	pid_t	pid;
	int		status;

	pid = fork();
	pipe_signals();
	if (pid == 0)
	{
		reset_signals();
		setup_redirection(cmd);
		exec_in_child(cmd, env);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			g_vars.exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			g_vars.exit_status = 128 + WTERMSIG(status);
		g_vars.in_pipe = 0;
	}
	else
		perror("minishell: fork failed");
}

void	ft_redict(t_command *cmd, char **env)
{
	if (is_builtin(cmd) == NOT_BUILT_IN)
	{
		redic_not_builtin(cmd, env);
	}
	else
		redic_builtin(cmd, env);
}

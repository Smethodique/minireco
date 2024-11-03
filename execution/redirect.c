/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nel-ouar <nel-ouar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 17:05:45 by stakhtou          #+#    #+#             */
/*   Updated: 2024/10/29 03:17:40 by nel-ouar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_path(char **args);

void	exec_in_child(t_command *cmd, char **env)
{
	cmd->args[0] = get_path(cmd->args);
	if (execve(cmd->args[0], cmd->args, env) == -1)
	{
		perror("minishell: execution failed");
		exit(127);
	}
}


void	redic_builtin(t_command *cmd, char **env)
{
	int	in;
	int	out;
	// int	new_in;
	int	new_out;

	in = dup(STDIN_FILENO);
	out = dup(STDOUT_FILENO);
	// new_in = get_in(cmd, STDIN_FILENO);
	new_out = get_out(cmd, STDOUT_FILENO);
	if (new_out == -1)
		return ;
	// dup2(new_in, STDIN_FILENO);
	dup2(new_out, STDOUT_FILENO);
	execute_builtin(cmd, env, is_builtin(cmd));
	dup2(in, STDIN_FILENO);
	dup2(out, STDOUT_FILENO);
	close(in);
	close(out);
	// if (new_in != STDIN_FILENO)
	// 	close(new_in);
	if (new_out != STDOUT_FILENO)
		close(new_out);
}
void	setup_redirection(t_command *cmd)
{
	// int	in;
	int	out;

	// in = get_in(cmd, STDIN_FILENO);
	out = get_out(cmd, STDOUT_FILENO);
	if (out == -1)
	{
		exit(1);
	}
	// if (in != STDIN_FILENO)
	// {
	// 	dup2(in, STDIN_FILENO);
	// 	close(in);
	// }
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
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
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
	}
	else
		perror("minishell: fork failed");
}

void	ft_redict(t_command *cmd, char **env)
{
	if (is_builtin(cmd) == NOT_BUILT_IN)
		redic_not_builtin(cmd, env);
	else
		redic_builtin(cmd, env);
}

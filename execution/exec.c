/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nel-ouar <nel-ouar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 17:05:45 by stakhtou          #+#    #+#             */
/*   Updated: 2024/10/29 03:57:02 by nel-ouar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	execute_cmd(char **cmd)
{
	pid_t	pid;
	char	*path;
	int		status;

	path = get_path(cmd);
	if (path == NULL)
	{
		printf("minishell: %s: command not found\n", cmd[0]);
		g_vars.exit_status = 127;
		return ;
	}
	pipe_signals();
	pid = fork();
	if (pid == 0)
	{
		setup_child_signals();
		execute_child(cmd, path);
	}
	else
	{
		waitpid(pid, &status, 0);
		handle_exit_status(status);
	}
	all_signals();
	free(path);
}

void	exec(t_command *cmd)
{
	char	**args;
	int		i;

	i = 0;
	args = malloc(sizeof(char *) * (double_pointer_len(cmd->args) + 1));
	if (!args)
		return ;
	while (cmd->args[i])
	{
		args[i] = ft_strdup(cmd->args[i]);
		i++;
	}
	args[i] = NULL;
	execute_cmd(args);
	ft_free(args);
}

void	execute_single_cmd(t_command *cmd, char **env)
{
	int	cmd_type;

	if (cmd == NULL || cmd->args == NULL || cmd->args[0] == NULL)
		return ;
	all_signals();
	cmd_type = is_builtin(cmd);
	if (cmd->redirections)
		ft_redict(cmd, env);
	else
	{
		if (cmd_type == -1)
			exec(cmd);
		else
			execute_builtin(cmd, env, cmd_type);
	}
}

void	setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGTSTP, SIG_DFL);
}

void	setup_redirections(t_command *cmd, int in_fd, int out_fd)
{
	int	red_out;

	red_out = get_out(cmd, out_fd);
	(void)in_fd;
	if (red_out != STDOUT_FILENO)
	{
		dup2(red_out, STDOUT_FILENO);
		close(red_out);
	}
	else if (out_fd != STDOUT_FILENO)
	{
		dup2(out_fd, STDOUT_FILENO);
		close(out_fd);
	}
}


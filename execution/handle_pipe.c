/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nel-ouar <nel-ouar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 17:05:45 by stakhtou          #+#    #+#             */
/*   Updated: 2024/10/29 03:57:46 by nel-ouar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	determine_fds(int *in_fd, int *out_fd, int in_pipe, int out_pipe)
{
	(void)in_pipe;
	if (g_vars.in_pipe != -1)
		*in_fd = in_pipe;
	else
		*in_fd = STDIN_FILENO;
	if (out_pipe != -1)
		*out_fd = out_pipe;
	else
		*out_fd = STDOUT_FILENO;
}
void	handle_command_execution(t_command *current, char **env)
{
	if (current->redirections)
		setup_redirection(current);
	if (is_builtin(current) != NOT_BUILT_IN)
		execute_builtin(current, env, is_builtin(current));
	else
		execute_external_command(current, env);
	exit(0);
}
void	close_pipe_fds(int i, t_command *current, int pipes[2][2])
{
	if (i > 0)
	{
		close(pipes[(i + 1) % 2][0]);
		close(pipes[(i + 1) % 2][1]);
	}
	if (current->next)
	{
		close(pipes[i % 2][0]);
	}
}

void	wait_for_children(pid_t *pids, int pipe_count)
{
	int	i;
	int	status;

	i = 0;
	while (i < pipe_count)
	{
		waitpid(pids[i], &status, 0);
		if (i == pipe_count - 1)
		{
			if (WIFEXITED(status))
				g_vars.exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
			{
				g_vars.exit_status = 128 + WTERMSIG(status);
				if (WTERMSIG(status) == SIGQUIT)
					write(1, "Quit\n", 5);
			}
		}
		i++;
	}
}

bool	initialize_pipe_data(t_pipe_data *data, t_command *commands)
{
	data->pipe_count = count_pipes(commands);
	data->pids = malloc(sizeof(pid_t) * data->pipe_count);
	return (data->pids != NULL);
}

bool	setup_next_pipe(int curr_pipe[2], t_command *current)
{
	if (current->next && pipe(curr_pipe) == -1)
	{
		perror("pipe failed");
		return (false);
	}
	return (true);
}

void	handle_pipes_in_child(int prev_pipe[2], int curr_pipe[2])
{
	if (prev_pipe[0] != -1)
	{
		dup2(prev_pipe[0], STDIN_FILENO);
		close(prev_pipe[0]);
		close(prev_pipe[1]);
	}
	if (curr_pipe[1] != -1)
	{
		dup2(curr_pipe[1], STDOUT_FILENO);
		close(curr_pipe[0]);
		close(curr_pipe[1]);
	}
}

void	setup_child_process(int prev_pipe[2], int curr_pipe[2],
		t_command *current, char **env)
{
	setup_child_signals();
	handle_pipes_in_child(prev_pipe, curr_pipe);
	handle_command_execution(current, env);
}

void	execute_external_command(t_command *current, char **env)
{
	char	*path;

	path = get_path(current->args);
	if (!path)
	{
		ft_putstr_fd("minishell: command not found: ", 2);
		ft_putstr_fd(current->args[0], 2);
		ft_putstr_fd("\n", 2);
		exit(127);
	}
	execve(path, current->args, env);
	free(path);
	exit(127);
}

void	close_prev_pipe(int prev_pipe[2])
{
	printf("closed prev pipe\n");
	if (prev_pipe[0] != -1)
	{
		close(prev_pipe[0]);
		close(prev_pipe[1]);
	}
}

void	update_prev_pipe(int prev_pipe[2], int curr_pipe[2])
{
	prev_pipe[0] = curr_pipe[0];
	prev_pipe[1] = curr_pipe[1];
}

void	wait_for_children_and_cleanup(t_pipe_data *data)
{
	wait_for_children(data->pids, data->pipe_count);
}

void	handle_pipes(t_command *commands, char **env)
{
	t_pipe_data	data;
	int			prev_pipe[2];
		int curr_pipe[2];
	pid_t		pid;

	if (!initialize_pipe_data(&data, commands))
		return ;
	pipe_signals();
	data.current = commands;
	data.i = 0;
	while (data.current)
	{
		if (!setup_next_pipe(curr_pipe, data.current))
			return ;
		printf("pipe: %d %d\n", curr_pipe[0], curr_pipe[1]);
		pid = fork();
		if (pid == 0)
			setup_child_process(prev_pipe, curr_pipe, data.current, env);
		data.pids[data.i++] = pid;
		close_prev_pipe(prev_pipe);
		update_prev_pipe(prev_pipe, curr_pipe);
		data.current = data.current->next;
	}
	wait_for_children_and_cleanup(&data);
	all_signals();
}

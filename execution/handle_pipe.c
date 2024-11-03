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
	if (g_vars.in_pipe != -1)
		*in_fd = in_pipe;
	else
		*in_fd = STDIN_FILENO;
	if (out_pipe != -1)
		*out_fd = out_pipe;
	else
		*out_fd = STDOUT_FILENO;
}

void	close_pipe_fds(int i, t_command *current, int pipes[2][2])
{
	if (i > 0)
		close(pipes[(i + 1) % 2][0]);
	if (current->next)
		close(pipes[i % 2][1]);
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

void	handle_pipes(t_command *commands, char **env)
{
	t_pipe_data	data;

	pipe_signals();
	data.pipe_count = count_pipes(commands);
	data.pids = malloc(sizeof(pid_t) * data.pipe_count);
	data.i = 0;
	data.current = commands;
	while (data.current)
	{
		if (data.current->next)
			setup_pipe(data.pipes, data.i);
		determine_fds(&data.in_fd, &data.out_fd, data.pipes[(data.i + 1)
			% 2][0], data.pipes[data.i % 2][1]);
		data.pids[data.i] = execute_piped_command(data.current, data.in_fd,
				data.out_fd, env);
		close_pipe_fds(data.i, data.current, data.pipes);
		data.current = data.current->next;
		data.i++;
	}
	wait_for_children(data.pids, data.pipe_count);
	free(data.pids);
	all_signals();
}

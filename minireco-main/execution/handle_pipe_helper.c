/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipe_helper.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iabboudi <iabboudi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 17:05:45 by stakhtou          #+#    #+#             */
/*   Updated: 2024/11/16 22:23:55 by iabboudi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	count_pipes(t_command *commands)
{
	int			pipe_count;
	t_command	*current;

	pipe_count = 0;
	current = commands;
	while (current)
	{
		pipe_count++;
		current = current->next;
	}
	return (pipe_count);
}

void	setup_pipe(int pipes[2][2], int i)
{
	if (pipe(pipes[i % 2]) == -1)
	{
		perror("pipe failed");
		exit(1);
	}
}

void	close_prev_pipe(int prev_pipe[2])
{
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
	free(data->pids);
}

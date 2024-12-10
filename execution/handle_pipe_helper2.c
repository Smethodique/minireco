/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipe_helper2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iabboudi <iabboudi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 21:52:15 by iabboudi          #+#    #+#             */
/*   Updated: 2024/12/02 05:55:07 by iabboudi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

	while (current->args && current->args[0] && current->args[0][0] == '\0')
		current->args++;
	if (!current->args || !current->args[0])
		exit(0);
	path = get_path(current->args);
	if (!path && current->name)
	{
		ft_putstr_fd("minishell: command not found: ", 2);
		ft_putstr_fd(current->args[0], 2);
		ft_putstr_fd("\n", 2);
		exit(127);
	}
	if (execve(path, current->args, env) == -1 && current->name)
	{
		perror("minishell: execution failed");
		exit(127);
	}
	free(path);
	free(current->args[0]);
	exit(0);
}

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

void close_pipe_fds(int i, t_command *current, int pipes[2][2])
{
    // Close read end of previous pipe if not first command
    if (i > 0)
    {
        close(pipes[(i + 1) % 2][0]);
        close(pipes[(i + 1) % 2][1]);
    }
    
    // Close write end of current pipe if not last command
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

void handle_pipes(t_command *commands, char **env)
{
    t_pipe_data data;
    int prev_pipe[2] = {-1, -1};

    pipe_signals();
    data.pipe_count = count_pipes(commands);
    data.pids = malloc(sizeof(pid_t) * data.pipe_count);
    if (!data.pids)
        return;

    data.i = 0;
    data.current = commands;

    while (data.current)
    {
        int curr_pipe[2] = {-1, -1};
        
        // Create pipe for all but the last command
        if (data.current->next && pipe(curr_pipe) == -1)
        {
            perror("pipe failed");
            free(data.pids);
            return;
        }

        pid_t pid = fork();
        if (pid == -1)
        {
            perror("fork failed");
            free(data.pids);
            return;
        }

        if (pid == 0)
        {
            // Child process
            setup_child_signals();

            // Set up input from previous pipe
            if (prev_pipe[0] != -1)
            {
                dup2(prev_pipe[0], STDIN_FILENO);
                close(prev_pipe[0]);
                close(prev_pipe[1]);
            }

            // Set up output to current pipe
            if (curr_pipe[1] != -1)
            {
                dup2(curr_pipe[1], STDOUT_FILENO);
                close(curr_pipe[0]);
                close(curr_pipe[1]);
            }

            // Handle command redirections
            if (data.current->redirections)
                setup_redirection(data.current);

            // Execute the command
            if (is_builtin(data.current) != NOT_BUILT_IN)
                execute_builtin(data.current, env, is_builtin(data.current));
            else
            {
                char *path = get_path(data.current->args);
                if (!path)
                {
                    ft_putstr_fd("minishell: command not found: ", 2);
                    ft_putstr_fd(data.current->args[0], 2);
                    ft_putstr_fd("\n", 2);
                    exit(127);
                }
                execve(path, data.current->args, env);
                free(path);
                exit(127);
            }
            exit(0);
        }
        data.pids[data.i] = pid;
        if (prev_pipe[0] != -1)
        {
            close(prev_pipe[0]);
            close(prev_pipe[1]);
        }
        if (curr_pipe[0] != -1)
        {
            prev_pipe[0] = curr_pipe[0];
            prev_pipe[1] = curr_pipe[1];
        }

        data.current = data.current->next;
        data.i++;
    }
	wait_for_children(data.pids, data.pipe_count);
    free(data.pids);
    all_signals();
}

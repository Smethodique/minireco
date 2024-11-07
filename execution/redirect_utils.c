/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nel-ouar <nel-ouar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 17:05:45 by stakhtou          #+#    #+#             */
/*   Updated: 2024/10/29 03:55:12 by nel-ouar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <wordexp.h>

int handle_redirection(t_redirection *redir, int fd_in)
{
    int new_fd;
    char *expanded;

    new_fd = fd_in;
    if (redir->type == INPUT || redir->type == HEREDOC)
    {
        if (new_fd != fd_in)
            close(new_fd);
        expanded = ft_strdup(redir->filename);
        if (redir->type == INPUT)
        {
            if (check_file(expanded, O_RDONLY) == -1)
            {
                g_vars.exit_status = 1;
                free(expanded);
                return (-1);
            }
        }
        new_fd = open(expanded, O_RDONLY);
        if (new_fd < 0)
        {
            perror("open");
            free(expanded);
            return (-1);
        }
        free(expanded);
        if (new_fd == -1)
            return (-1);
    }
    return (new_fd);
}

int get_in(t_redirection *redir, int fd_in)
{
    t_redirection    *current;
    int             new_fd;

	current = redir;
	new_fd = fd_in;
	while (current)
	{
		new_fd = handle_redirection(current, new_fd);
		if (new_fd == -1)
			return (-1);
		current = current->next;
	}
	return (new_fd);
}


int	get_out(t_command *cmd, int fd_out)
{
	t_redirection	*red;
	int				new_fd;
	char			*expanded;

	red = cmd->redirections;
	new_fd = fd_out;
	while (red)
	{
		if (red->type == OUTPUT || red->type == APPEND)
		{
			expanded = expand_variables(ft_strdup(red->filename));
			 if (ft_strlen(expanded) == 0 || check_directory(get_directory_path(expanded), 1) == -1)
			 	return (ft_putstr_fd("ambiguous redirect\n", 2), -1);
			if (red->type == OUTPUT)
				new_fd = open(expanded, O_WRONLY | O_CREAT | O_TRUNC, P);
			else
				new_fd = open(expanded, O_WRONLY | O_CREAT | O_APPEND, P);
			dup2(new_fd, fd_out);
		}
		else if (red->type == INPUT || red->type == HEREDOC)
			if (get_in(red, STDIN_FILENO) < 0)
				return (new_fd);
		red = red->next;
	}
	return (new_fd);
}

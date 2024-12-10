/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iabboudi <iabboudi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 17:05:45 by stakhtou          #+#    #+#             */
/*   Updated: 2024/12/07 23:42:54 by iabboudi         ###   ########.fr       */
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

int	handle_redirection(t_redirection *redir, int fd_in)
{
	int		new_fd;
	char	*expanded;

	new_fd = fd_in;
	if (redir->type == INPUT || redir->type == HEREDOC)
	{
		expanded = ft_strdup(redir->filename);
		new_fd = open(expanded, O_RDONLY);
		if (new_fd < 0)
		{
			free(expanded);
			return (-1);
		}
		free(expanded);
	}
	return (new_fd);
}

int	get_in(t_redirection *redir, int fd_in)
{
	t_redirection	*current;
	int				new_fd;

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

int	handle_output_redirection(t_redirection *red, int fd_out)
{
	int		new_fd;
	char	*expanded;
	char	*strdup;

	strdup = ft_strdup(red->filename);
	expanded = expand_variables(strdup);
	if (!expanded)
	{
		ft_putstr_fd("ambiguous redirect\n", 2);
		free(expanded);
		free(strdup);
		return (-1);
	}
	if (red->type == OUTPUT)
		new_fd = open(expanded, O_WRONLY | O_CREAT | O_TRUNC, P);
	else
		new_fd = open(expanded, O_WRONLY | O_CREAT | O_APPEND, P);
	if (new_fd < 0)
	{
		return (free(expanded), free(strdup), -1);
	}
	dup2(new_fd, fd_out);
	free(expanded);
	free(strdup);
	return (new_fd);
}

int	get_out(t_command *cmd, int fd_out)
{
	t_redirection	*red;
	int				new_fd;

	red = cmd->redirections;
	new_fd = fd_out;
	while (red)
	{
		if (red->type == OUTPUT || red->type == APPEND)
		{
			new_fd = handle_output_redirection(red, fd_out);
			if (new_fd == -1)
				return (-1);
		}
		else if (red->type == INPUT || red->type == HEREDOC)
		{
			if (get_in(red, STDIN_FILENO) < 0)
				return (new_fd);
		}
		red = red->next;
	}
	return (new_fd);
}

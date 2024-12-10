/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_herdoc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakhtou <stakhtou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 17:05:45 by stakhtou          #+#    #+#             */
/*   Updated: 2024/10/21 03:26:40 by stakhtou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>

void	init_heredoc(t_heredoc *hd, const char *delimiter, int expand_vars)
{
	hd->delimiter = delimiter;
	hd->unquoted_delimiter = remove_quotes((char *)delimiter);
	if (!hd->unquoted_delimiter)
	{
		free(hd->unquoted_delimiter);
		return ;
	}
	hd->is_quoted = (ft_strcmp(delimiter, hd->unquoted_delimiter) != 0);
	hd->content = NULL;
	hd->content_size = 0;
	hd->content_capacity = 0;
	hd->expand_vars = expand_vars;
}

int	process_line(t_heredoc *hd)
{
	hd->processed_line = hd->line;
	if (hd->expand_vars && !hd->is_quoted)
	{
		hd->processed_line = expand_env_vars(hd->line);
		if (!hd->processed_line)
		{
			free(hd->line);
			free(hd->content);
			free(hd->unquoted_delimiter);
			return (0);
		}
	}
	hd->line_len = ft_strlen(hd->processed_line);
	return (1);
}
// make your own rl_hook_func_t

int	realloc_content(t_heredoc *hd)
{
	char	*new_content;
	size_t	new_size;

	new_size = hd->content_size + hd->line_len + 2;
	if (new_size > hd->content_capacity)
	{
		hd->content_capacity = new_size * 2;
		new_content = ft_realloc(hd->content, hd->content_capacity);
		if (!new_content)
		{
			free(hd->line);
			if (hd->expand_vars && hd->processed_line != hd->line)
				free(hd->processed_line);
			free(hd->content);
			free(hd->unquoted_delimiter);
			return (0);
		}
		hd->content = new_content;
	}
	return (1);
}

static char	*read_heredoc_content(t_heredoc *hd)
{
	hd->content = ft_strdup("");
	hd->content_capacity = 1;
	while (1)
	{
		hd->line = readline("> ");
		if (!hd->line || g_vars.heredoc_interrupted)
		{
			free(hd->line);
		}
		if (ft_strcmp(hd->line, hd->unquoted_delimiter) == 0)
		{
			free(hd->line);
			break ;
		}
		if (!process_line(hd) || !realloc_content(hd))
			return (NULL);
		ft_strcpy(hd->content + hd->content_size, hd->processed_line);
		hd->content_size += hd->line_len;
		hd->content[hd->content_size++] = '\n';
		if (hd->expand_vars && hd->processed_line != hd->line)
			free(hd->processed_line);
		free(hd->line);
	}
	hd->content[hd->content_size] = '\0';
	return (hd->content);
}

char	*handle_heredoc(const char *delimiter, int expand_vars)
{
	t_heredoc	hd;
	char		*result;
	pid_t		pid;
	int			status;
	int			pipe_fd[2];

	if (pipe(pipe_fd) == -1)
		return (NULL);
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		init_heredoc(&hd, delimiter, expand_vars);
		result = read_heredoc_content(&hd);
		write_and_free(pipe_fd, &hd, result);
	}
	close(pipe_fd[1]);
	waitpid(pid, &status, 0);
	result = read_from_pipe(pipe_fd[0]);
	if (WIFEXITED(status))
		g_vars.exit_status = WEXITSTATUS(status);
	if (WIFSIGNALED(status))
		g_vars.heredoc_interrupted = 1;
	return (close(pipe_fd[0]), result);
}

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
	while (1)
	{
		hd->line = readline("> ");
		if (!hd->line || g_vars.heredoc_interrupted)
		{
			free(hd->line);
			break ;
		}
		if (ft_strcmp(hd->line, hd->unquoted_delimiter) == 0
			|| !process_line(hd) || !realloc_content(hd))
		{
			free(hd->line);
			break ;
		}
		ft_strcpy(hd->content + hd->content_size, hd->processed_line);
		hd->content_size += hd->line_len;
		hd->content[hd->content_size++] = '\n';
		if (hd->expand_vars && hd->processed_line != hd->line)
			free(hd->processed_line);
	}
	return (hd->content);
}

char	*handle_heredoc(const char *delimiter, int expand_vars)
{
	t_heredoc	hd;
	void		(*old_handler)(int);
	char		*result;

	g_vars.heredoc_interrupted = 0;
	old_handler = signal(SIGINT, sigint_handlerh);
	init_heredoc(&hd, delimiter, expand_vars);
	result = read_heredoc_content(&hd);
	signal(SIGINT, old_handler);
	dup2(g_vars.khbi, 0);
	if (g_vars.heredoc_interrupted)
	{
		free(hd.content);
		free(hd.unquoted_delimiter);
		return (NULL);
	}
	if (result)
		result[hd.content_size] = '\0';
	free(hd.unquoted_delimiter);
	return (result);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_read.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iabboudi <iabboudi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 17:05:45 by stakhtou          #+#    #+#             */
/*   Updated: 2024/12/12 22:43:06 by iabboudi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*read_from_pipe(int fd)
{
	char	*result;
	char	buffer[4096];
	ssize_t	bytes_read;
	size_t	result_len;
	size_t	result_cap;

	result = ft_strdup("");
	result_len = 0;
	result_cap = 1;
	while (1)
	{
		bytes_read = read(fd, buffer, 4096);
		if (bytes_read == 0)
			break ;
		if (result_len + bytes_read + 1 > result_cap)
		{
			result_cap = (result_len + bytes_read + 1) * 2;
			result = ft_realloc(result, result_cap);
		}
		ft_memcpy(result + result_len, buffer, bytes_read);
		result_len += bytes_read;
		result[result_len] = '\0';
	}
	return (result);
}

void	write_and_free(int pipe_fd[2], t_heredoc *hd, char *result)
{
	write(pipe_fd[1], result, ft_strlen(result));
	free(result);
	g_vars.exit_status = 130;
	g_vars.heredoc_interrupted = 1;
	exit(130);
}

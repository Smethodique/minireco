/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakhtou <stakhtou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 10:02:46 by stakhtou          #+#    #+#             */
/*   Updated: 2024/06/05 07:22:19 by stakhtou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

char	*readex_line(int fd, char *line)
{
	char	*buff;
	ssize_t	rd;

	rd = 1;
	buff = (char *)malloc(sizeof(char) * BUFFER_SIZE + 1);
	if (!buff)
		return (NULL);
	while (!ft_strchr(line, '\n') && rd > 0)
	{
		rd = read(fd, buff, BUFFER_SIZE);
		if (rd == -1)
		{
			free(buff);
			return (NULL);
		}
		buff[rd] = '\0';
		line = ft_strjoin(line, buff);
	}
	free(buff);
	return (line);
}

char	*zyada_mn_rass_l7me9(char *line)
{
	char	*str;
	int		i;

	if (line == NULL || line[0] == '\0')
		return (NULL);
	i = 0;
	while (line[i] != '\0' && line[i] != '\n')
		i++;
	if (line[i] == '\n')
		i++;
	str = (char *)malloc(i + 1);
	if (!str)
		return (NULL);
	i = 0;
	while (line[i] != '\0' && line[i] != '\n')
	{
		str[i] = line[i];
		i++;
	}
	if (line[i] == '\n')
		str[i++] = '\n';
	str[i] = '\0';
	return (str);
}

char	*khli_gha_lmouhim(char *line)
{
	char	*str;
	int		i;
	int		j;

	i = 0;
	if (!line)
		return (NULL);
	while (line[i] != '\0' && line[i] != '\n')
		i++;
	if (!line[i])
	{
		free(line);
		return (NULL);
	}
	str = (char *)malloc(ft_strlen(line) - i + 1);
	if (!str)
		return (NULL);
	i++;
	j = 0;
	while (line[i] != '\0')
		str[j++] = line[i++];
	str[j] = '\0';
	free(line);
	return (str);
}

char	*get_next_line(int fd)
{
	static char	*line;
	char		*next;

	if (BUFFER_SIZE <= 0 || fd < 0 || BUFFER_SIZE > 2147483647)
		return (NULL);
	line = readex_line(fd, line);
	next = zyada_mn_rass_l7me9(line);
	line = khli_gha_lmouhim(line);
	return (next);
}

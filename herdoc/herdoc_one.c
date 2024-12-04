/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc_one.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakhtou <stakhtou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 17:05:45 by stakhtou          #+#    #+#             */
/*   Updated: 2024/10/21 18:42:23 by stakhtou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	myrand(void)
{
	static unsigned int	seed = 1;

	seed = (seed * 1103515245 + 12345) % 32768;
	return (seed);
}

int	create_temp_file(char *template)
{
	int	fd;
	int	i;

	while (1)
	{
		i = 0;
		while (i < 6)
		{
			template[ft_strlen(template) - 6 + i] = 'a' + myrand() % 26;
			i++;
		}
		fd = open(template, O_RDWR | O_CREAT | O_EXCL, 0600);
		if (fd >= 0)
			break ;
	}
	return (fd);
}

int	my_mkstemp(char *template)
{
	int		fd;
	char	*p;
	int		len;
	int		i;

	len = ft_strlen(template);
	p = NULL;
	if (len < 6)
		return (-1);
	p = template + len - 6;
	i = 0;
	while (i < 6)
	{
		if (p[i] != 'X')
			return (-1);
		i++;
	}
	fd = create_temp_file(template);
	if (fd < 0)
	{
		perror("mkstemp");
		return (-1);
	}
	return (fd);
}

void	*ft_calloc(size_t count, size_t size)
{
	void	*ptr;

	ptr = malloc(count * size);
	if (ptr == NULL)
		return (ptr);
	ft_bzero(ptr, size * count);
	return (ptr);
}

void	*ft_realloc(void *ptr, size_t size)
{
	void	*new_ptr;

	if (!ptr)
		return (malloc(size));
	if (!size)
	{
		free(ptr);
		return (NULL);
	}
	new_ptr = malloc(size);
	if (!new_ptr)
		return (NULL);
	ft_memcpy(new_ptr, ptr, size);
	free(ptr);
	return (new_ptr);
}

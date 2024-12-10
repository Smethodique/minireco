/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iabboudi <iabboudi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 22:15:49 by stakhtou          #+#    #+#             */
/*   Updated: 2024/12/07 00:43:14 by iabboudi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*f_strcpy(char *dst, const char *src)
{
	int	i;

	i = 0;
	while (src[i] != '\0')
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}

char	*ft_strdup(const char *s1)
{
	char	*ptr;
	size_t	i;

	if (!s1)
		return (NULL);
	i = 0;
	while (s1[i] != '\0')
	{
		i++;
	}
	ptr = malloc(sizeof(char) * (i + 1));
	if (ptr == NULL)
	{
		return (NULL);
	}
	f_strcpy(ptr, s1);
	return (ptr);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakhtou <stakhtou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 16:55:12 by stakhtou          #+#    #+#             */
/*   Updated: 2023/11/18 00:57:08 by stakhtou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stddef.h>
#include <stdlib.h>

static char	*allocate_empty_string(void)
{
	char	*ptr;

	ptr = (char *)malloc(1);
	if (ptr == NULL)
		return (NULL);
	ptr[0] = '\0';
	return (ptr);
}

static size_t	calculate_len(const char *s, unsigned int start, size_t len)
{
	size_t	restchar;
	size_t	actual_len;

	restchar = ft_strlen(s + start);
	actual_len = 0;
	while (actual_len < len && actual_len < restchar)
	{
		actual_len++;
	}
	return (actual_len);
}

static char	*copy(const char *s, unsigned int start, size_t actual_len)
{
	char	*ptr;

	ptr = (char *)malloc(actual_len + 1);
	if (ptr == NULL)
		return (NULL);
	ft_memcpy(ptr, s + start, actual_len);
	ptr[actual_len] = '\0';
	return (ptr);
}

char	*ft_substr(const char *s, unsigned int start, size_t len)
{
	size_t	actual_len;

	if (s == NULL)
	{
		return (NULL);
	}
	if (start >= ft_strlen(s))
	{
		return (allocate_empty_string());
	}
	actual_len = calculate_len(s, start, len);
	return (copy(s, start, actual_len));
}

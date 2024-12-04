/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakhtou <stakhtou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/12 14:23:23 by stakhtou          #+#    #+#             */
/*   Updated: 2023/11/18 01:01:51 by stakhtou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

static char	*empty_str(void)
{
	char	*empty_str;

	empty_str = (char *)malloc(1);
	if (empty_str == NULL)
		return (NULL);
	empty_str[0] = '\0';
	return (empty_str);
}

static char	*trim_string(char const *s1, size_t bl, size_t el)
{
	size_t	ml;
	char	*ptr;
	size_t	i;

	ml = el - bl;
	ptr = (char *)malloc(ml + 1);
	if (ptr == NULL)
		return (NULL);
	i = 0;
	while (i < ml)
	{
		ptr[i] = s1[bl + i];
		i++;
	}
	ptr[ml] = '\0';
	return (ptr);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	bl;
	size_t	el;

	if (s1 == NULL || set == NULL)
		return (NULL);
	bl = 0;
	el = ft_strlen(s1);
	while (s1[bl] != '\0')
	{
		if (ft_strchr(set, s1[bl]) == NULL)
			break ;
		bl++;
	}
	while (el > 0)
	{
		if (ft_strchr(set, s1[el - 1]) == NULL)
			break ;
		el--;
	}
	if (bl >= el)
	{
		return (empty_str());
	}
	return (trim_string(s1, bl, el));
}

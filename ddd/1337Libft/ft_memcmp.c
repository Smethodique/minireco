/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakhtou <stakhtou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 20:13:32 by stakhtou          #+#    #+#             */
/*   Updated: 2023/11/11 15:57:44 by stakhtou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t	i;
	int		result;

	i = 0;
	result = 0;
	while (i < n && (((unsigned char *)s1)[i] == ((unsigned char *)s2)[i]))
	{
		i++;
	}
	if (i < n)
	{
		result = ((unsigned char *)s1)[i] - ((unsigned char *)s2)[i];
	}
	return (result);
}

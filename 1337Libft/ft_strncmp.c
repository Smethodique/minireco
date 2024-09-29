/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakhtou <stakhtou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 17:27:48 by stakhtou          #+#    #+#             */
/*   Updated: 2023/11/11 15:58:43 by stakhtou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;
	int		result;

	i = 0;
	while (i < n && (s1[i] != '\0' || s2[i] != '\0'))
	{
		result = (unsigned char)s1[i] - (unsigned char)s2[i];
		if (result != 0)
			return (result);
		i++;
	}
	return (0);
}

int ft_strcmp(const char *s1, const char *s2)
{
	return (ft_strncmp(s1, s2, ft_strlen(s1) + 1));
}

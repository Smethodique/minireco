/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakhtou <stakhtou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 16:57:06 by stakhtou          #+#    #+#             */
/*   Updated: 2023/11/24 21:05:16 by stakhtou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *b, int c, size_t len)
{
	char	d;
	size_t	i;

	i = 0;
	d = (unsigned char)c;
	while (i < len)
	{
		((unsigned char *)b)[i] = d;
		i++;
	}
	return (b);
}

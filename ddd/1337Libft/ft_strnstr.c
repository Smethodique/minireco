/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakhtou <stakhtou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 20:48:38 by stakhtou          #+#    #+#             */
/*   Updated: 2024/06/05 08:45:03 by stakhtou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *hayStack, const char *needle, size_t len)
{
	size_t	i;
	size_t	needle_len;

	if (!hayStack && len == 0)
		return (NULL);
	needle_len = ft_strlen(needle);
	if (needle_len == 0)
		return ((char *)hayStack);
	i = 0;
	while (hayStack[i] != '\0' && i + needle_len - 1 < len)
	{
		if (ft_strncmp(&hayStack[i], needle, needle_len) == 0)
			return ((char *)&hayStack[i]);
		i++;
	}
	return (NULL);
}

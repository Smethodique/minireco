/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakhtou <stakhtou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 15:27:24 by stakhtou          #+#    #+#             */
/*   Updated: 2023/11/16 17:51:36 by stakhtou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
#include <stdlib.h>

static int	calclen(int n)
{
	int	len;

	len = 0;
	if (n <= 0)
	{
		len++;
		n = -n;
		while (n != 0)
		{
			n = n / 10;
			len++;
		}
	}
	else if (n > 0)
	{
		while (n != 0)
		{
			n = n / 10;
			len++;
		}
	}
	return (len);
}

static char	*ktaba(int len, int i, char *str, int n)
{
	while (len > i)
	{
		len--;
		if (n < 0)
		{
			str[len] = 48 + (n % 10) * (-1);
			n = n / 10;
		}
		else
		{
			str[len] = 48 + (n % 10);
			n = n / 10;
		}
	}
	return (str);
}

char	*ft_itoa(int n)
{
	int		len;
	char	*str;
	int		i;

	i = 0;
	len = calclen(n);
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	if (n < 0)
	{
		str[0] = '-';
		i++;
	}
	ktaba(len, i, str, n);
	str[len] = '\0';
	return (str);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putdec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakhtou <stakhtou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 20:27:55 by stakhtou          #+#    #+#             */
/*   Updated: 2024/03/10 00:27:49 by stakhtou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static void	ft_putchar2(char c)
{
	write(1, &c, 1);
}

int	ft_putdec(long num)
{
	int	len;

	len = ft_calclen(num);
	if (num == 0)
		ft_putchar2('0');
	if (num > 0 && num <= 9)
		ft_putchar2(48 + num);
	else if (num > 9)
	{
		ft_putdec(num / 10);
		ft_putdec(num % 10);
	}
	else if (num < 0)
	{
		ft_putchar2('-');
		num = -num;
		if (num > 0 && num <= 9)
			ft_putchar2(48 + num);
		else if (num > 9)
		{
			ft_putdec(num / 10);
			ft_putdec(num % 10);
		}
	}
	return (len);
}

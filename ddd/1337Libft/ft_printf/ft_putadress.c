/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putadress.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakhtou <stakhtou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 09:54:21 by stakhtou          #+#    #+#             */
/*   Updated: 2024/03/10 00:27:31 by stakhtou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	ft_putadress(void *add)
{
	int	len;

	ft_putstr("0x");
	len = 2 + ft_puthex((size_t)add);
	return (len);
}

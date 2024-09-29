/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakhtou <stakhtou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 20:27:55 by stakhtou          #+#    #+#             */
/*   Updated: 2024/03/10 14:27:39 by stakhtou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static int	ft_spec(const char type, va_list args, int *len)
{
	if (type == 'c')
		return (ft_putchar(va_arg(args, int), len));
	else if (type == 's')
		return (*len += ft_putstr(va_arg(args, char *)));
	else if (type == 'd' || type == 'i')
		return (*len += ft_putdec(va_arg(args, int)));
	else if (type == 'u')
		return (*len += ft_putundec(va_arg(args, unsigned int)));
	else if (type == 'x')
		return (*len += ft_puthex(va_arg(args, unsigned int)));
	else if (type == 'X')
		return (*len += ft_putuphex(va_arg(args, unsigned int)));
	else if (type == 'p')
		return (*len += ft_putadress(va_arg(args, void *)));
	else if (type == '%')
		ft_putchar('%', len);
	return (0);
}

int	ft_printf(const char *printer, ...)
{
	int		i;
	int		len;
	va_list	args;

	if (!printer)
		return (0);
	i = 0;
	len = 0;
	va_start(args, printer);
	while (printer[i] != '\0')
	{
		if (printer[i] == '%' && printer[i + 1] == '\0')
			return (0);
		else if (printer[i] != '%')
			ft_putchar(printer[i], &len);
		else
		{
			i++;
			ft_spec(printer[i], args, &len);
		}
		i++;
	}
	va_end(args);
	return (len);
}

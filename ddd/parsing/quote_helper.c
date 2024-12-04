/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakhtou <stakhtou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 17:05:45 by stakhtou          #+#    #+#             */
/*   Updated: 2024/10/21 18:42:37 by stakhtou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <string.h>

int	is_quoted(const char *str)
{
	int	in_single_quotes;
	int	in_double_quotes;
	int	i;

	in_single_quotes = 0;
	in_double_quotes = 0;
	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '\'' && !in_double_quotes)
		{
			in_single_quotes = !in_single_quotes;
		}
		else if (str[i] == '"' && !in_single_quotes)
		{
			in_double_quotes = !in_double_quotes;
		}
		i++;
	}
	return (in_single_quotes || in_double_quotes);
}

char	*remove_single_quotes(const char *str)
{
	int		len;
	char	*result;
	int		j;
	int		in_single_quotes;
	int		i;

	i = 0;
	len = ft_strlen(str);
	result = malloc(len + 1);
	j = 0;
	in_single_quotes = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '\'')
			in_single_quotes = !in_single_quotes;
		else
			result[j++] = str[i++];
	}
	result[j] = '\0';
	if (in_single_quotes)
	{
		g_vars.exit_status = 2;
		return (free(result), ft_strdup(str));
	}
	return (result);
}

int	calculate_quote_num(const char *input, int len, int *j, int *p)
{
	int	i;

	*j = 0;
	*p = 0;
	i = 0;
	while (i < len)
	{
		if (input[i] == '\'')
		{
			(*j)++;
		}
		else if (input[i] == '"')
		{
			(*p)++;
		}
		i++;
	}
	if (*j % 2 != 0 || *p % 2 != 0)
	{
		return (0);
	}
	return (1);
}

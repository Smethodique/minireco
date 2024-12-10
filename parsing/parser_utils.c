/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iabboudi <iabboudi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 17:05:45 by stakhtou          #+#    #+#             */
/*   Updated: 2024/12/06 23:37:39 by iabboudi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

size_t	ft_strnlen(const char *s, size_t maxlen)
{
	size_t	len;

	len = 0;
	while (len < maxlen && s[len])
		len++;
	return (len);
}

char	*ft_strndup(const char *s, size_t n)
{
	size_t	len;
	char	*dup;

	if (s == NULL)
	{
		return (NULL);
	}
	len = ft_strnlen(s, n);
	dup = (char *)malloc(len + 1);
	if (dup == NULL)
		return (NULL);
	ft_strncpy(dup, s, len);
	dup[len] = '\0';
	return (dup);
}

char	*ft_strjoin_char(char *s, char c)
{
	char	*new_str;
	int		len;

	len = ft_strlen(s);
	new_str = (char *)malloc(len + 2);
	if (!new_str)
		return (NULL);
	ft_memcpy(new_str, s, len);
	new_str[len] = c;
	new_str[len + 1] = '\0';
	return (new_str);
}

char	*get_env_value(char *name, char **env)
{
	int		i;
	size_t	name_len;

	i = 0;
	name_len = ft_strlen(name);
	if (!name || !env)
		return (NULL);
	while (env[i])
	{
		if (ft_strncmp(env[i], name, name_len) == 0 && env[i][name_len] == '=')
		{
			return (env[i] + name_len + 1);
		}
		i++;
	}
	return (NULL);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iabboudi <iabboudi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 17:05:45 by stakhtou          #+#    #+#             */
/*   Updated: 2024/12/07 02:36:11 by iabboudi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	*exit_code(void)
{
	static int	c;

	return (&c);
}

int	ft_getter(void)
{
	int	*i;

	i = exit_code();
	return (*i);
}

void	ft_setter(int value)
{
	int	*i;

	i = exit_code();
	*i = value;
}

int	double_pointer_len(char **str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
		i++;
	return (i);
}

void	increment_shlvl(char **env, bool is_shlvl)
{
	int		i;
	int		shlvl;
	char	*tmp;
	char	*new_value;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "SHLVL=", 6) == 0)
		{
			shlvl = atoi(env[i] + 6);
			shlvl++;
			tmp = ft_itoa(shlvl);
			new_value = ft_strjoin("SHLVL=", tmp);
			free(tmp);
			if (!new_value)
				return ;
			if (is_shlvl)
				free(env[i]);
			env[i] = new_value;
			return ;
		}
		i++;
	}
}

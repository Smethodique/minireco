/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakhtou <stakhtou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 17:05:45 by stakhtou          #+#    #+#             */
/*   Updated: 2024/10/24 16:42:07 by stakhtou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	export_helper(char *cmd, char ***env, int len)
{
	int		x;
	char	*new_var;

	x = check_env(cmd, *env);
	if (ft_strchr(cmd, '+') && ft_strchr(cmd, '='))
	{
		append_export(cmd, env, len);
		return ((void)len);
	}
	if (x && ft_strchr(cmd, '=') && (length(cmd) == length((*env)[x])))
	{
		if ((*env)[x])
			(*env)[x] = ft_strdup(cmd);
	}
	else if (!check_env(cmd, *env))
	{
		if (ft_strchr(cmd, '='))
		{
			new_var = ft_strdup(cmd);
			add_to_envp(env, new_var);
			free(new_var);
		}
		else
			add_to_envp(env, cmd);
	}
}

int	check_export(char *cmd)
{
	int	i;

	if (!ft_isalpha(cmd[0]) && cmd[0] != '_')
		return (ft_putstr_fd("Minishell: not a valid identifier\n", 2), 0);
	i = -1;
	while (cmd[++i])
	{
		if (cmd[i] == '+' && cmd[i + 1] == '=')
		{
			if (i == 0)
				return (ft_putstr_fd("Minishell: not a valid identifier\n", 2),
					0);
			break ;
		}
		else if (cmd[i] == '+')
			return (ft_putstr_fd("Minishell: export: not a valid identifier\n",
					2), 0);
		else if (cmd[i] == '=')
			break ;
		else if (!ft_isalnum(cmd[i]) && cmd[i] != '_')
			return (ft_putstr_fd("Minishell:not a valid identifier\n", 2), 0);
	}
	if (i == 0)
		return (ft_putstr_fd("Minishell:  not a valid identifier\n", 2), 0);
	return (1);
}

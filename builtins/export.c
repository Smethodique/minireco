/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iabboudi <iabboudi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 17:05:45 by stakhtou          #+#    #+#             */
/*   Updated: 2024/12/12 08:53:51 by iabboudi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	export_helper(char *cmd, char ***env, int len)
{
	int		x;
	char	*new_var;

	x = check_env(cmd, *env);
	if (ft_strchr(cmd, '+') )
	{
		g_vars.exit_status = 1;
		return (ft_putstr_fd(" export: not a valid identifier\n", 2), 0);
	}
	if (x &&(length(cmd) == length((*env)[x])))
	{
		(*env)[x] = ft_strdup(cmd);
		gc_add(0, (*env)[x]);
	}
	else if (!check_env(cmd, *env))
	{
			new_var = cmd;
			g_vars.env_locked = 1;
			add_to_env(env, new_var);
	}
	return (1);
}

int	check_export(char *cmd)
{
	int	i;

	if (!cmd || !*cmd)
		return (ft_putstr_fd("Minishell: export: not a valid identifier\n", 2),
			0);
	if (!ft_isalpha(cmd[0]) && cmd[0] != '_')
		return (ft_putstr_fd("Minishell: not a valid identifier\n", 2),
			g_vars.exit_status = 1, 0);
	i = 0;
	while (cmd[i] && cmd[i] != '=')
	{
		if (!ft_isalnum(cmd[i]) && cmd[i] != '_' && cmd[i] != '+')
			return (ft_putstr_fd("Minishell: not a valid identifier\n", 2),
				g_vars.exit_status = 1, 0);
		if (cmd[i] == '+' && cmd[i + 1] != '=')
			return (ft_putstr_fd("Minishell: export: not a valid identifier\n",
					2), g_vars.exit_status = 1, 0);
		i++;
	}
	if (i == 0)
		return (ft_putstr_fd("Minishell: not a valid identifier\n", 2),
			g_vars.exit_status = 1, 0);
	return (g_vars.exit_status = 0, 1);
}

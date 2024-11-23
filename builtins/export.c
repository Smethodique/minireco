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
	int         i;
	
	i=0;
	printf("cmd: %s\n", cmd);
	x = check_env(cmd, *env);
	while(cmd[i] != '=')
		i++;
	if (cmd[i + 1] == '+'  && cmd[i + 2] == '=')
	{
		append_export(cmd, env, len);
		return ((void)len);
	}
	printf("x: %d\n", x);
	if (x)
	{
		free((*env)[x]);
		printf("cmd1: %s\n", cmd);
		(*env)[x] = ft_strdup(cmd);
	}
	else
	{
		new_var = ft_strdup(cmd);
		printf("cmd2: %s\n", cmd);
		add_to_env(env, new_var);
		free(new_var);
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

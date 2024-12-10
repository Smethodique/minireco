/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iabboudi <iabboudi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 17:05:45 by stakhtou          #+#    #+#             */
/*   Updated: 2024/12/07 21:08:57 by iabboudi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_num(char *str)
{
	int	i;

	i = 0;
	if (str[0] == '+' || str[0] == '-')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

void	custom_exit(t_command *cmd, char **env)
{
	int	len;

	len = double_pointer_len(cmd->args);
	if (len == 1)
	{
		exit(g_vars.exit_status);
	}
	else if (len == 2 && is_num(cmd->args[1]))
	{
		exit(ft_atoi(cmd->args[1]));
	}
	else if (len > 2)
	{
		ft_putstr_fd("exit: too many arguments\n", 2);
		g_vars.exit_status = 1;
	}
	else
	{
		ft_putstr_fd("exit: numeric argument required\n", 2);
		g_vars.exit_status = 2;
		exit(g_vars.exit_status);
	}
}

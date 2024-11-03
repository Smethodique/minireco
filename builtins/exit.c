/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nel-ouar <nel-ouar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 17:05:45 by stakhtou          #+#    #+#             */
/*   Updated: 2024/10/24 15:27:37 by nel-ouar         ###   ########.fr       */
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
	ft_putstr_fd("exit\n", 2);
	if (len >= 2 && !is_num(cmd->args[1]))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(cmd->args[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		_exit(255);
	}
	else if (len <= 2)
	{
		if (cmd->args[1])
		{
			ft_setter(ft_atoi(cmd->args[1]));
		}
		_exit(ft_getter());
	}
	else
	{
		ft_setter(1);
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
	}
}

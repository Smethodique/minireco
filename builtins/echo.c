/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nel-ouar <nel-ouar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 17:05:45 by stakhtou          #+#    #+#             */
/*   Updated: 2024/10/24 15:27:47 by nel-ouar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_n_option(char *arg)
{
	int	i;

	i = 0;
	if (arg && arg[i] == '-' && arg[i + 1] == 'n')
		i += 2;
	else if (!arg)
		return (2);
	else
		return (0);
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	first_non_option(char **env)
{
	int	i;

	i = 1;
	while (env[i] && is_n_option(env[i]))
		i++;
	return (i);
}

int	echo(t_command *cmd, char **env)
{
	int	i;

	(void)env;
	i = first_non_option(cmd->args);
	if (is_n_option(cmd->args[i]) == 2)
		return (0);
	while (cmd->args[i])
	{
		ft_putstr_fd(cmd->args[i], 1);
		if (cmd->args[i + 1])
			ft_putstr_fd(" ", 1);
		i++;
	}
	if (!is_n_option(cmd->args[1]))
		ft_putstr_fd("\n", 1);
	g_vars.exit_status = 0;
	if (g_vars.heredoc_interrupted)
	{
		g_vars.exit_status = 130;
		g_vars.heredoc_interrupted = 0;
	}
	return (0);
}

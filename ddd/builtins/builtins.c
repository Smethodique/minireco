/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iabboudi <iabboudi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 17:05:45 by stakhtou          #+#    #+#             */
/*   Updated: 2024/12/02 07:36:13 by iabboudi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_cmd_exit(t_command *cmd)
{
	int	i;

	i = 0;
	if (!cmd->args[1])
	{
		g_vars.exit_status = 0;
		exit(g_vars.exit_status);
	}
	while (cmd->args[1][i])
	{
		if (!ft_isdigit(cmd->args[1][i]))
		{
			ft_putstr_fd("exit: numeric argument required\n", 2);
			g_vars.exit_status = 2;
			exit(g_vars.exit_status);
			return (0);
		}
		i++;
	}
	if (cmd->arg_count > 2)
	{
		ft_putstr_fd("exit: too many arguments\n", 2);
		return (g_vars.exit_status = 1, 0);
	}
	return (1);
}

void	ft_exit(t_command *cmd)
{
	int	i;
	int	status;

	i = 0;
	status = 0;
	ft_putstr_fd("exit\n", 1);
	if (!handle_cmd_exit(cmd))
		return ;
	if (cmd->arg_count == 2)
	{
		while (cmd->args[1][i])
		{
			if (!ft_isdigit(cmd->args[1][i]))
			{
				printf("minishell: exit: %s: numeric argument required\n",
					cmd->args[1]);
				g_vars.exit_status = 2;
				exit(g_vars.exit_status);
			}
			i++;
		}
		status = ft_atoi(cmd->args[1]);
		g_vars.exit_status = status;
	}
	exit(g_vars.exit_status);
}

void	export(t_command *cmd)
{
	int	i;
	int	len;
	char *new_var;	

	if (cmd->args[1])
	{
		i = 1;
		while (cmd->args[i])
		{
			if (check_export(cmd->args[i]) == 1)
			{
				len = 0;
				while (g_vars.env[len])
					len++;
				export_helper(cmd->args[i], &g_vars.env, len);
			}
			i++;
		}
	}
	else
	{
		i = -1;
		while (g_vars.env[++i])
			print_export(g_vars.env[i]);
	}
}

void	execute_builtin(t_command *cmd, char **environment, int index)
{
	if (index == 1)
		cd(cmd);
	else if (index == 2)
		export(cmd);
	else if (index == 3)
		unset(cmd);
	else if (index == 4)
		echo(cmd, environment);
	else if (index == 5)
		pwd(cmd, environment);
	else if (index == 6)
		ft_exit(cmd);
	else if (index == 10)
	{
		env(cmd);
	}
	else
		g_vars.exit_status = 0;
}

int	is_builtin(t_command *cmd)
{
	if (ft_strcmp(cmd->args[0], "cd") == 0)
	{
		if (cmd->args[0] == NULL)
			return (-1);
		return (1);
	}
	else if (ft_strcmp(cmd->args[0], "env") == 0)
		return (10);
	else if (ft_strcmp(cmd->args[0], "export") == 0)
		return (2);
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
		return (3);
	else if (ft_strcmp(cmd->args[0], "echo") == 0)
		return (4);
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		return (5);
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
		return (6);
	else
	{
		return (-1);
	}
}

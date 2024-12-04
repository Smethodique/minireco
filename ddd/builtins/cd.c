/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iabboudi <iabboudi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 17:05:45 by stakhtou          #+#    #+#             */
/*   Updated: 2024/11/16 22:29:21 by iabboudi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	with_path(char **argv)
{
	char	*new_path;

	init_current_dir();
	new_path = build_path(g_vars.current_dir, argv[1]);
	if (g_vars.current_dir)
		update_env_variable(g_vars.env, "OLDPWD=", g_vars.current_dir);
	if (chdir(new_path) == -1)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(argv[1], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		g_vars.exit_status = 1;
		free(new_path);
		return ;
	}
	update_current_dir(new_path);
	free(new_path);
}

void	home(char *oldpwd)
{
	char	*home_path;

	home_path = get_env_value("HOME", g_vars.env);
	if (!home_path)
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", 2);
		g_vars.exit_status = 1;
		return ;
	}
	if (oldpwd)
		update_env_variable(g_vars.env, "OLDPWD=", oldpwd);
	if (chdir(home_path) == -1)
	{
		ft_putstr_fd("minishell: cd: HOME not accessible\n", 2);
		g_vars.exit_status = 1;
		return ;
	}
	update_current_dir(home_path);
}

void	update_current_dir(char *new_path)
{
	if (g_vars.current_dir)
		free(g_vars.current_dir);
	g_vars.current_dir = ft_strdup(new_path);
	update_env_variable(g_vars.env, "PWD=", g_vars.current_dir);
}

void	go_oldpwd(void)
{
	if (!g_vars.saved_oldpwd)
	{
		ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
		g_vars.exit_status = 1;
		return ;
	}
	if (g_vars.current_dir)
		update_env_variable(g_vars.env, "OLDPWD=", g_vars.current_dir);
	if (chdir(g_vars.saved_oldpwd) == -1)
	{
		ft_putstr_fd("minishell: cd: OLDPWD not accessible\n", 2);
		g_vars.exit_status = 1;
		return ;
	}
	update_current_dir(g_vars.saved_oldpwd);
	ft_putstr_fd(g_vars.saved_oldpwd, 1);
	ft_putstr_fd("\n", 1);
}

void	cd(t_command *cmd)
{
	char	*current;

	g_vars.exit_status = 0;
	if (!g_vars.current_dir)
		init_current_dir();
	current = get_current_dir();
	if (current)
	{
		update_env_variable(g_vars.env, "OLDPWD=", current);
		free(current);
	}
	if (cmd->arg_count == 1)
		home(g_vars.current_dir);
	else if (cmd->arg_count == 2)
	{
		if (ft_strcmp(cmd->args[1], "-") == 0)
			go_oldpwd();
		else
			with_path(cmd->args);
	}
	else
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		g_vars.exit_status = 1;
	}
}

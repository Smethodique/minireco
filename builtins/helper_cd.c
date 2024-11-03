/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_cd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nel-ouar <nel-ouar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 17:05:45 by stakhtou          #+#    #+#             */
/*   Updated: 2024/10/28 21:31:38 by nel-ouar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdbool.h>

static char	*get_home_path(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "HOME=", 5) == 0)
			return (env[i] + 5);
		i++;
	}
	return (NULL);
}

static void	expand_tilde(char **path, char *home)
{
	char	*expanded_path;

	if (!home)
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", 2);
		g_vars.exit_status = 1;
		return ;
	}
	if ((*path)[1] == '/' || (*path)[1] == '\0')
		expanded_path = ft_strjoin(home, *path + 1);
	else
		expanded_path = ft_strdup(*path);
	if (!expanded_path)
	{
		g_vars.exit_status = 1;
		return ;
	}
	free(*path);
	*path = expanded_path;
}

static char	*get_target_path(t_command *cmd, char **env)
{
	char	*path;
	char	*home;

	if (!cmd->args[1] || ft_isspace(cmd->args[1][0]) ||
		cmd->args[1][0] == '\0' || ft_strncmp(cmd->args[1], "--", 3) == 0)
	{
		home = get_home_path(env);
		if (!home || *home == '\0' || ft_isspace(*home))
			return (ft_putstr_fd("minishell: cd: HOME not set\n", 2), NULL);
		return (ft_strdup(home));
	}
	else if (ft_strncmp(cmd->args[1], "-", 2) == 0)
	{
		path = get_env_value("OLDPWD", env);
		if (!path)
			return (ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2), NULL);
		return (ft_putendl_fd(path, 1), ft_strdup(path));
	}
	path = ft_strdup(cmd->args[1]);
	if (path[0] == '~' && g_vars.exit_status == 0)
	{
		home = get_home_path(env);
		expand_tilde(&path, home);
	}
	return (path);
}

void	cd(t_command *cmd, char **env)
{
	char	*path;

	if (!cmd->args)
	{
		g_vars.exit_status = 1;
		return ;
	}
	if (cmd->args[1] && cmd->args[2])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		g_vars.exit_status = 1;
		return ;
	}
	path = get_target_path(cmd, env);
	if (!path)
	{
		g_vars.exit_status = 1;
		return ;
	}
	if (!change_dir(env, path))
		g_vars.exit_status = 1;
	else
		g_vars.exit_status = 0;
	free(path);
}

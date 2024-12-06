/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_cd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iabboudi <iabboudi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 17:05:45 by stakhtou          #+#    #+#             */
/*   Updated: 2024/12/02 09:54:44 by iabboudi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"



void update_env_variable(char **env, char *var, char *value)
{
	int i;
	char new_var[4096];  

	i = 0;
	ft_strcpy(new_var, var);
	ft_strcat(new_var, value);

	while (env[i])
	{
		if (ft_strncmp(env[i], var, ft_strlen(var)) == 0)
		{
			ft_strcpy(env[i], new_var);
			return;
		}
		i++;
	}
}

char	*get_current_dir(void)
{
	char	*cwd;
	char	*pwd;

	cwd = getcwd(NULL, 0);
	if (cwd)
		return (cwd);
	if (g_vars.current_dir)
		return (ft_strdup(g_vars.current_dir));
	else
	{
		pwd = get_env_value("PWD", g_vars.env);
		if (pwd)
			return (ft_strdup(pwd));
	}
	return (NULL);
}

void	init_current_dir(void)
{
	char	*cwd;
	char	*pwd;

	cwd = getcwd(NULL, 0);
	if (cwd)
		update_current_dir(cwd);
	else if (!g_vars.current_dir)
	{
		pwd = get_env_value("PWD", g_vars.env);
		if (pwd)
			update_current_dir(pwd);
	}
	if (cwd)
		free(cwd);
}

char	*build_path(char *base, char *path)
{
	char	*result;
	char	*temp;
	int		i;

	if (path[0] == '/')
		return (ft_strdup(path));
	if (ft_strcmp(path, "..") == 0)
	{
		temp = ft_strjoin(base, "/..");
		return (temp);
	}
	result = ft_strjoin(base, "/");
	temp = result;
	result = ft_strjoin(result, path);
	free(temp);
	i = 0;
	while (result[i])
		i++;
	if (result[i - 1] == '/')
		result[i - 1] = '\0';
	return (result);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iabboudi <iabboudi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 17:05:45 by stakhtou          #+#    #+#             */
/*   Updated: 2024/12/02 09:53:03 by iabboudi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_current_dire(void)
{
	char	*cwd;
	char	*oldpwd;
	char	*pwd;
	char	*newpath;

	cwd = getcwd(NULL, 0);
	if (cwd)
		return (cwd);
	if (g_vars.current_dir)
		return (ft_strdup(g_vars.current_dir));
	else
	{
		oldpwd = get_env_value("OLDPWD", g_vars.env);
		pwd = get_env_value("PWD", g_vars.env);
		newpath = ft_strjoin(oldpwd, pwd);
		return (newpath);
	}
}

int	pwd(t_command *cmd, char **env)
{
	char	*current_dir;
	char	*oldpwd;
	int		i;

	(void)cmd;
	(void)env;
	current_dir = getcwd(NULL, 0);
	if (current_dir == NULL)
		current_dir = get_current_dire();
	if (!current_dir)
	{
		oldpwd = ft_strjoin("OLDPWD=", current_dir);
		i = 0;
		while (g_vars.env[i])
		{
			if (ft_strncmp(g_vars.env[i], "OLDPWD=", 7) == 0)
			{
				g_vars.env[i++] = oldpwd;
				break ;
			}
		}
	}
	else
		return (printf("%s\n", current_dir), free(current_dir), 0);
}

size_t	length(char *s)
{
	size_t	i;

	if (!s)
		return (0);
	i = 0;
	while (s[i] && s[i] != '=')
		i++;
	return (i);
}

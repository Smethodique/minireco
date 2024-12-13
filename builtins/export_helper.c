/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iabboudi <iabboudi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 17:05:45 by stakhtou          #+#    #+#             */
/*   Updated: 2024/12/13 02:28:19 by iabboudi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_export(char *env)
{
 	   	int		i;
	char	*pwd;
	char	*env_value;

	i = 0;
	while (g_vars.env[i])
	{
		env_value = ft_strdup(g_vars.env[i]);
		gc_add(0, env_value);
		if (strncmp(g_vars.env[i], "PWD=", 4) == 0)
		{
			pwd = getcwd(NULL, 0);
			g_vars.env[i] = ft_strjoin("PWD=", pwd);
			gc_add(0, g_vars.env[i]);
			gc_add(0, pwd);
		}
		printf("declare -x %s\n", env_value);
		i++;
	}
}

int	check_env(char *cmd, char **env)
{
	int	i;

	i = 0;
	if (cmd == NULL)
		return (0);
	while (env[i])
	{
		if (!ft_strncmp(env[i], cmd, length(cmd)))
			return (i);
		else
			i++;
	}
	return (0);
}

void	add_to_envp(char ***env, char *new_var)
{
	int		len;
	char	**new_env;
	int		i;

	len = double_pointer_len(*env);
	new_env = malloc(sizeof(char *) * (len + 2));
	if (new_env)
	{
		i = 0;
		while (i < len)
		{
			new_env[i] = (*env)[i];
			i++;
		}
		new_env[i] = ft_strdup(new_var);
		new_env[i + 1] = NULL;
		*env = new_env;
	}
}

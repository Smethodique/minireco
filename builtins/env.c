/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iabboudi <iabboudi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 17:05:45 by stakhtou          #+#    #+#             */
/*   Updated: 2024/12/13 02:38:55 by iabboudi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_env(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		free(env[i]);
		i++;
	}
	free(env);
}

void	add_to_env(char ***env, char *new_var)
{
	int		len;
	int		i;
	char	**new_env;

	len = double_pointer_len(*env);
	new_env = malloc(sizeof(char *) * (len + 2));
	gc_add(0, new_env);
	i = 0;
	while (i < len)
	{
		new_env[i] = ft_strdup((*env)[i]);
		gc_add(0, new_env[i]);
		i++;
	}

	new_env[i] = ft_strdup(new_var);
	gc_add(0, new_env[i]);
	new_env[i + 1] = NULL;
	*env = new_env;
}

void	print_env(void)
{
	int		i;
	char	*pwd;
	char	*pwd_var;

	i = 0;
	while (g_vars.env[i])
	{
		if (strncmp(g_vars.env[i], "PWD=", 4) == 0)
		{
			pwd = getcwd(NULL, 0);
			if (pwd)
			{
				pwd_var = ft_strjoin("PWD=", pwd);
				if (pwd_var)
				{
					printf("%s\n", pwd_var);
					free(pwd);
					free(pwd_var);
				}
			}
		}
		else if (strchr(g_vars.env[i], '='))
			printf("%s\n", g_vars.env[i]);
		i++;
	}
}

void	env(t_command *cmd)
{
	int		i;
	char	**new_env;

	i = 1;
	new_env = NULL;
	if (cmd->args && cmd->args[i])
		return ;
	else
		new_env = g_vars.env;
	while (cmd->args[i] && ft_strchr(cmd->args[i], '='))
		add_to_env(&new_env, cmd->args[i++]);
	g_vars.env = new_env;
	if (cmd->args[i] == NULL)
	{
		print_env();
		ft_setter(0);
	}
	if (g_vars.env == NULL || g_vars.env[0] == NULL)
		create_env();
	ft_setter(0);
}

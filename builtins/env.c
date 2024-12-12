/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iabboudi <iabboudi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 17:05:45 by stakhtou          #+#    #+#             */
/*   Updated: 2024/12/07 02:24:00 by iabboudi         ###   ########.fr       */
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
	i = 0;
	while (i < len)
	{
		new_env[i] = ft_strdup((*env)[i]);
		i++;
	}
	new_env[i] = ft_strdup(new_var);
	new_env[i + 1] = NULL;
	if (g_vars.env_allocated)
	{
		g_vars.env_allocated = 0;
		free_env(*env);
	}
	*env = new_env;
	
	g_vars.env_allocated = 1;
}

void	print_env(void)
{
	int	i;
	int env_len;	

	i = 0;
	env_len = double_pointer_len(g_vars.env);
	if(env_len == 3)
	{
		//delete the old one and create a new one
		free_env(g_vars.env);
		g_vars.env = create_env();

	}
	while (g_vars.env[i])
	{
		if(!g_vars.env[i])
			break;
		ft_putstr_fd(g_vars.env[i], 1);
		ft_putstr_fd("\n", 1);
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

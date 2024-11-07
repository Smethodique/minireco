/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nel-ouar <nel-ouar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 17:05:45 by stakhtou          #+#    #+#             */
/*   Updated: 2024/10/29 04:13:16 by nel-ouar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	add_to_env(char ***env, char *new_var)
{
	int		len;
	int		i;
	char	**new_env;

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
		new_env[i] = new_var;
		new_env[i + 1] = NULL;
		*env = new_env;
	}
}

void	print_env(void)
{
	int	j;

	j = 0;
	while (g_vars.env[j])
	{
		if (ft_strcmp(g_vars.env[j],
				"PATH=/usr/local/sbin:/usr/local/bin:"
				"/usr/sbin:/usr/bin:/sbin:/bin") == 0)
		{
			j++;
			continue ;
		}
		if(ft_strchr(g_vars.env[j],'='))
		printf("%s\n", g_vars.env[j]);
		j++;
	}
}

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

void	env(t_command *cmd)
{
	int		i;
	char	**new_env;

	i = 1;
	new_env = NULL;
	if (cmd->args && cmd->args[1] && ft_strcmp(cmd->args[1], "-i") == 0)
		i++;
	if (g_vars.env == NULL || g_vars.env[0] == NULL)
		new_env = create_env();
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

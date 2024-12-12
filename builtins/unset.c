/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iabboudi <iabboudi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 15:27:07 by nel-ouar          #+#    #+#             */
/*   Updated: 2024/12/07 02:52:33 by iabboudi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	find_env_var(char *var_name)
{
	int		i;
	size_t	var_len;
	size_t	curr_len;

	i = 0;
	if (!var_name)
		return (-1);
	var_len = length(var_name);
	while (g_vars.env[i])
	{
		curr_len = length(g_vars.env[i]);
		if (curr_len == var_len && !ft_strncmp(g_vars.env[i], var_name,
				var_len))
		{
			return (i);
		}
		i++;
	}
	return (-1);
}

static void	remove_env_var(char *var_name)
{
	int	i;
	int	env_len;
	int	env_index;

	env_len = 0;
	while (g_vars.env[env_len])
		env_len++;
	env_index = find_env_var(var_name);
	if (env_index == -1)
		return ;
	i = env_index;
	while (i < env_len - 1)
	{
		g_vars.env[i] = g_vars.env[i + 1];
		i++;
	}
	g_vars.env[env_len - 1] = NULL;
}

static int	is_valid_identifier(const char *str)
{
	int	i;

	if (!str || !*str)
		return (0);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

void	add_env_var(const char *new_var)
{
	int		env_len;
	char	**new_env;
	int		i;

	env_len = 0;
	while (g_vars.env[env_len])
		env_len++;
	new_env = malloc(sizeof(char *) * (env_len + 2));
	if (!new_env)
		return ;
	i = 0;
	while (i < env_len)
	{
		new_env[i] = g_vars.env[i];
		i++;
	}
	new_env[env_len] = ft_strdup(new_var);
	new_env[env_len + 1] = NULL;
	g_vars.env = new_env;
}

void	unset(t_command *cmd)
{
	int		i;
	char	*var_name;

	i = 1;
	while (cmd->args[i])
	{
		var_name = cmd->args[i];
		if (var_name[0] == '$')
			var_name++;
		if (is_valid_identifier(var_name))
		{
			remove_env_var(var_name);
		}
		else
		{
			ft_putstr_fd("minishell: unset: `", 2);
			ft_putstr_fd(cmd->args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			g_vars.exit_status = 0;
		}
		i++;
	}
}

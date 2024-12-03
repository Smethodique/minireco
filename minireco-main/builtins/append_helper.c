/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   append_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakhtou <stakhtou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 17:05:45 by stakhtou          #+#    #+#             */
/*   Updated: 2024/10/24 16:42:28 by stakhtou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	init_append(t_append *append, char *cmd)
{
	append->plus_pos = ft_strchr(cmd, '+');
	append->equals_pos = ft_strchr(cmd, '=');
	if (!append->plus_pos || !append->equals_pos || append->plus_pos
		+ 1 != append->equals_pos)
		return ;
	append->var_name = ft_substr(cmd, 0, append->plus_pos - cmd);
	append->var_value = ft_strdup(append->equals_pos + 1);
	append->i = 0;
}

int	is_var_match(char *env_var, char *var_name)
{
	return (ft_strncmp(env_var, var_name, ft_strlen(var_name)) == 0
		&& (env_var[ft_strlen(var_name)] == '='
			|| env_var[ft_strlen(var_name)] == '\0'));
}

void	handle_existing_with_value(t_append *append, char ***env)
{
	append->new_value = ft_strjoin(append->temp + 1, append->var_value);
	append->temp = ft_strjoin(append->var_name, "=");
	(*env)[append->i] = ft_strjoin(append->temp, append->new_value);
	free(append->temp);
	free(append->new_value);
}

void	handle_existing_no_value(t_append *append, char ***env)
{
	append->temp = ft_strjoin(append->var_name, "=");
	(*env)[append->i] = ft_strjoin(append->temp, append->var_value);
	free(append->temp);
}

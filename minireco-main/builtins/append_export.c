/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   append_export.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakhtou <stakhtou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 17:05:45 by stakhtou          #+#    #+#             */
/*   Updated: 2024/10/24 03:53:36 by stakhtou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	create_new_var(t_append *append, char ***env)
{
	append->temp = ft_strjoin(append->var_name, "=");
	append->new_value = ft_strjoin(append->temp, append->var_value);
	add_to_envp(env, append->new_value);
	free(append->temp);
	free(append->new_value);
}

void	cleanup_append(t_append *append)
{
	free(append->var_name);
	free(append->var_value);
}

int	handle_existing_var(t_append *append, char ***env)
{
	while ((*env)[append->i])
	{
		if (is_var_match((*env)[append->i], append->var_name))
		{
			append->temp = ft_strchr((*env)[append->i], '=');
			if (append->temp)
				handle_existing_with_value(append, env);
			else
				handle_existing_no_value(append, env);
			cleanup_append(append);
			return (1);
		}
		append->i++;
	}
	return (0);
}

void	append_export(char *cmd, char ***env, int len)
{
	t_append	append;

	(void)len;
	init_append(&append, cmd);
	if (!append.plus_pos || !append.equals_pos || append.plus_pos
		+ 1 != append.equals_pos)
		return ;
	if (!handle_existing_var(&append, env))
	{
		create_new_var(&append, env);
		cleanup_append(&append);
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nel-ouar <nel-ouar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 17:05:45 by stakhtou          #+#    #+#             */
/*   Updated: 2024/10/24 15:27:32 by nel-ouar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_export(char *env)
{
	int	i;

	i = 0;
	printf("declare -x ");
	if (ft_strchr(env, '='))
	{
		while (env[i] != '=')
			printf("%c", env[i++]);
		printf("=\"");
		i++;
		printf("%s\"\n", (env + i));
	}
	else
		printf("%s\n", env);
}

int	check_env(char *cmd, char **env)
{
	int	i;

	i = 0;
	if (cmd == NULL)
		return (0);
	while (env[i])
	{
		if (!ft_strncmp(env[i], cmd, length(env[i])))
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

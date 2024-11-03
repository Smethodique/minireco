/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nel-ouar <nel-ouar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 17:05:45 by stakhtou          #+#    #+#             */
/*   Updated: 2024/10/28 21:31:07 by nel-ouar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	update_env_variable(char **env, const char *var, const char *value)
{
	char	*new_var;
	int		i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], var, ft_strlen(var)) == 0)
		{
			new_var = ft_strjoin((char *)var, (char *)value);
			env[i] = new_var;
			return ;
		}
		i++;
	}
}

void	update_wds(char **env, const char *wd)
{
	update_env_variable(env, "OLDPWD=", get_env_value("PWD", env));
	update_env_variable(env, "PWD=", wd);
}

bool	chdir_errno_mod(const char *path)
{
	if (errno == ESTALE)
		errno = ENOENT;
	ft_putstr_fd("minishell: cd: ", 2);
	ft_putstr_fd((char *)path, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(strerror(errno), 2);
	return (false);
}

bool	change_dir(char **env, char *path)
{
	char	*ret;
	char	cwd[PATH_MAX];
	char	*tmp;

	ret = NULL;
	if (chdir(path) != 0)
		return (chdir_errno_mod(path));
	ret = getcwd(cwd, PATH_MAX);
	if (!ret)
	{
		ft_putstr_fd("minishell: cd: getcwd: cannot access"
			"parent directories: No such file or directory\n",
			2);
		ret = ft_strjoin(get_env_value("PWD", env), "/");
		tmp = ret;
		ret = ft_strjoin(tmp, path);
		free(tmp);
	}
	else
	{
		ret = ft_strdup(cwd);
	}
	update_wds(env, ret);
	free(ret);
	return (true);
}

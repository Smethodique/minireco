/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_expantion.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iabboudi <iabboudi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 17:05:45 by stakhtou          #+#    #+#             */
/*   Updated: 2024/12/07 21:08:17 by iabboudi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_directory_path(const char *filename)
{
	char	*last_slash;

	last_slash = ft_strrchr(filename, '/');
	if (!last_slash)
		return (ft_strdup("."));
	return (ft_substr(filename, 0, last_slash - filename));
}

int	check_directory(const char *path, int check_write)
{
	struct stat	st;

	if (stat(path, &st) == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd((char *)path, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (-1);
	}
	if (!S_ISDIR(st.st_mode))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd((char *)path, 2);
		ft_putstr_fd(": Not a directory\n", 2);
		return (-1);
	}
	if (check_write && access(path, W_OK) == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd((char *)path, 2);
		ft_putstr_fd(": Permission denied\n", 2);
		return (-1);
	}
	return (0);
}

int	check_file(const char *path, int mode)
{
	struct stat	st;

	if (mode != O_RDONLY && !ft_strchr(path, '/'))
		return (0);
	if (stat(path, &st) != -1)
	{
		if (S_ISDIR(st.st_mode))
		{
			return (ft_putstr_fd("minishell: ", 2), ft_putstr_fd((char *)path,
					2), ft_putstr_fd(": Is a directory\n", 2), -1);
		}
		if ((mode == O_RDONLY && access(path, R_OK) == -1) || (mode != O_RDONLY
				&& access(path, W_OK) == -1))
		{
			return (ft_putstr_fd("minishell: ", 2), ft_putstr_fd((char *)path,
					2), ft_putstr_fd(": Permission denied\n", 2), -1);
		}
	}
	else if (mode == O_RDONLY)
	{
		return (-1);
	}
	return (0);
}

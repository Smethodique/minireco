/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_helper2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iabboudi <iabboudi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 22:27:18 by iabboudi          #+#    #+#             */
/*   Updated: 2024/11/16 22:27:33 by iabboudi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	execute_child_h(char **cmd, char *path)
{
	ft_putstr_fd("minishell: ", 2);
	char *free_path =  get_path(cmd);
	if (errno == EACCES && free_path == NULL)
	{
		ft_putstr_fd(path, 2);
		free(path);
		free(free_path);
		ft_putstr_fd(": Permission denied\n", 2);
		exit(126);
	}
	else
	{
		ft_putstr_fd(cmd[0], 2);
		if (cmd[0][0] == '.' && cmd[0][1] == '/')
		{
			ft_putstr_fd(": permission denied\n", 2);
			free(path);
			free(free_path);
			exit(126);
		}
		ft_putstr_fd(": command not found\n", 2);
		free(path);
		exit(127);
	}
}

void	execute_child(char **cmd, char *path)
{
	struct stat	path_stat;
	if (stat(path, &path_stat) == -1)
	{
		perror(cmd[0]);
		free(path);
		exit(127);
	}
	if (S_ISDIR(path_stat.st_mode))
	{
	  	printf("minishell: %s: is a directory\n", cmd[0]);
		free(path);
		exit(126);
	}
	if (execve(path, cmd, g_vars.env) == -1)
	{

		execute_child_h(cmd, path);
	}
	free(path);
	exit(1);
}

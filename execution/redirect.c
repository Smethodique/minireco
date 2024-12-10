/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iabboudi <iabboudi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 17:05:45 by stakhtou          #+#    #+#             */
/*   Updated: 2024/12/07 23:09:21 by iabboudi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	read_input_loop(void)
{
	char	*str;

	while (1)
	{
		str = readline("");
		if (str == NULL)
		{
			sigint_handler(SIGINT);
			signal(SIGINT, sigint_handler);
			exit(1);
		}
		free(str);
		{
			sigint_handler(SIGINT);
			signal(SIGINT, sigint_handler);
			exit(1);
		}
		sigint_handler(SIGINT);
		signal(SIGINT, sigint_handler);
		exit(1);
		sigint_handler(SIGINT);
		signal(SIGINT, sigint_handler);
		exit(1);
	}
}

void	exec_in_child(t_command *cmd, char **env)
{
	cmd->args[0] = get_path(cmd->args);
	if (g_vars.flag_check == 2)
	{
		read_input_loop();
	}
	if (cmd->name && execve(cmd->args[0], cmd->args, env) == -1)
	{
		perror("minishell: execution failed");
		exit(127);
	}
	exit(0);
}

void	redic_builtin(t_command *cmd, char **env)
{
	int	in;
	int	out;
	int	new_in;
	int	new_out;

	in = dup(STDIN_FILENO);
	out = dup(STDOUT_FILENO);
	new_in = get_in(cmd->redirections, STDIN_FILENO);
	new_out = get_out(cmd, STDOUT_FILENO);
	if (new_out == -1)
		return ;
	dup2(new_in, STDIN_FILENO);
	dup2(new_out, STDOUT_FILENO);
	execute_builtin(cmd, env, is_builtin(cmd));
	dup2(in, STDIN_FILENO);
	dup2(out, STDOUT_FILENO);
	close(in);
	close(out);
	if (new_in != STDIN_FILENO)
		close(new_in);
	if (new_out != STDOUT_FILENO)
		close(new_out);
}

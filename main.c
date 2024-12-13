/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iabboudi <iabboudi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 17:05:45 by stakhtou          #+#    #+#             */
/*   Updated: 2024/12/12 22:45:22 by iabboudi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_global_vars	g_vars;

void	process_linee(char *line, char **env)
{
	t_token		*tokens;
	t_command	*commands;

	tokens = tokenize_input(line);
	if (tokens)
	{
		commands = parse_tokens(tokens);
		if (commands)
		{
			if ((commands->name || commands->redirections))
			{
				if (commands->next)
					handle_pipes(commands, env);
				else if (commands->name || commands->redirections)
					execute_single_cmd(commands, env);
			}
			else
				return ;
			free_command_list(commands);
		}
		free_tokens(tokens);
	}
	return ;
}

void	init_shell(char **env)
{
	char	*line;

	while (1)
	{
		all_signals();
		line = readline("\033[3;32mminishell$ \033[0m");
		if (!line)
		{
			printf("exit\n");
			if(g_vars.khbi != 655)
				gc_free_all();
			break ;
		}
		if (line)
		{
			process_linee(line, env);
			add_history(line);
		}
	}
}

char	**create_env(void)
{
	char	**env;
	char	*pwd;

	env = malloc(sizeof(char *) * 4);
	if (!env)
		return (NULL);
	pwd = getcwd(NULL, 0);
	static char env_storage[4][256];
	ft_strlcpy(env_storage[0], "PWD=", 256);
	ft_strlcat(env_storage[0], pwd, 256);
	ft_strlcpy(env_storage[1], "SHLVL=1", 256);
	ft_strlcpy(env_storage[2], "_=/usr/bin/env", 256);
	env[0] = env_storage[0];
	env[1] = env_storage[1];
	env[2] = env_storage[2];
	env[3] = NULL;
	free(pwd);
	return (env);
}

void	handle_exit_status(int status)
{
	if (WIFEXITED(status))
		g_vars.exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		g_vars.exit_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGQUIT)
			write(1, "Quit\n", 5);
	}
}

int	main(int argc, char **argv, char **env)
{
	bool	isunset;

	(void)argc;
	(void)argv;
	g_vars.khbi = 1;
	g_vars.heredoc_interrupted = 0;
	g_vars.in_pipe = 0;
	g_vars.env_allocated = 0;
	g_vars.env_locked = 0;
	g_vars.error_printed = 0;
	isunset = false;
	if (env == NULL || env[0] == NULL)
	{
		env = create_env();
		gc_add_double(0, (void **)env);
		g_vars.khbi = 655;
		isunset = true;
	}
	g_vars.env = env;
	increment_shlvl(g_vars.env, isunset);
	init_shell(g_vars.env);
	gc_add(0, g_vars.env);
	return (g_vars.exit_status);
}

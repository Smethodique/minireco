/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nel-ouar <nel-ouar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 17:05:45 by stakhtou          #+#    #+#             */
/*   Updated: 2024/10/29 04:02:10 by nel-ouar         ###   ########.fr       */
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
			if (commands->next)
			{
				handle_pipes(commands, env);
			}
			else
				execute_single_cmd(commands, env);
			free_command_list(commands);
		}
		free_tokens(tokens);
	}
}

void	init_shell(char **env)
{
	char	*line;

	while (1)
	{
		line = readline("\033[3;32mminishell$ \033[0m");
		if (line == NULL)
			break ;
		if (ft_strlen(line) > 0)
		{
			process_linee(line, env);
			add_history(line);
		}
		free(line);
	}
}

char	**create_env(void)
{
	char	**env;
	char	cwd[1024];

	env = malloc(sizeof(char *) * 5);
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("getcwd() error");
		return (NULL);
	}
	env[0] = ft_strjoin("PWD=", cwd);
	env[1] = ft_strdup("SHLVL=1");
	env[2] = ft_strdup("_=./minishell");
	env[3] = ft_strdup("PATH=/usr/local/sbin:/usr/local/bin:"
			"/usr/sbin:/usr/bin:/sbin:/bin");
	env[4] = NULL;
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
	(void)argc;
	(void)argv;
	g_vars.khbi = -1;
	g_vars.heredoc_interrupted = 0;
	g_vars.in_pipe = 0;
	if (env == NULL || env[0] == NULL)
	{
		env = create_env();
	}
	g_vars.env = env;
	increment_shlvl(g_vars.env);
	all_signals();
	init_shell(env);
	return (g_vars.exit_status);
}

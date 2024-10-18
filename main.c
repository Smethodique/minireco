#include "minishell.h"

t_global_vars	g_vars;


void	init_shell(char **env)
{
	char		*line;
	t_token		*tokens;
	t_command	*commands;

	while (1)
	{
		line = readline("minishell$ ");
		if (line == NULL)
			break ;
		if (ft_strlen(line) > 0)
		{
			tokens = tokenize_input(line);
			if (tokens)
			{
				print_tokens(tokens); 
				
				
				commands = parse_tokens(tokens);
				if (commands)
				{
					
					print_command_list(commands);
						// Print command list for debuggin
					if (commands->next)
						handle_pipes(commands, env);
							// Add this function call for pipe handling
					else
					{
						execute_single_cmd(commands, env);
					}
					free_command_list(commands);
				}
				free_tokens(tokens);
			}
			add_history(line);
		}
		free(line);
	}
}


char **create_env()
{
   
      char **env = malloc(sizeof(char *) * 5);  // 4 variables + NULL terminator
    char cwd[1024];

    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd() error");
        return NULL;
    }

    env[0] = ft_strjoin("PWD=", cwd);
    env[1] = ft_strdup("SHLVL=1");
    env[2] = ft_strdup("_=./minishell");
    env[3] = ft_strdup("PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin");
    env[4] = NULL;

    return env;
}
	

int	main(int argc, char **argv, char **env)
{

	(void)argc;
	(void)argv;

	g_vars.env = env;
	if(env == NULL || env[0] == NULL)
	{
		env = create_env();
	}
	all_signals();
	init_shell(env);

	return (g_vars.exit_status);
}
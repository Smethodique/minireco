#include "minishell.h"
t_global_vars g_vars;


void init_shell(char **env)
{
    while(1)
    {
        char *line = readline("minishell$ ");
        if (line == NULL)
            break;
        if (ft_strlen(line) > 0)
        {
            t_token *tokens = tokenize_input(line);
            if (tokens)
            {
                print_tokens(tokens);  // Print tokens for debugging
                t_command *commands = parse_tokens(tokens);
                if (commands)
                {
                   
                    print_command_list(commands);  // Print command list for debuggin
                    if (commands->next)
                        handle_pipes(commands, env);  // Add this function call for pipe handling
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

int main(int argc, char **argv, char **env)
{
    (void)argc;
    (void)argv;
    
    // Initialize signal handlers
     g_vars.env = env;
    all_signals();
    init_shell(env);
    
    return g_vars.exit_status;
}
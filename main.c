#include "minishell.h"
int g_exit_status;

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
                    print_command_list(commands);  // Print command list for debugging
                    
                    // Check if there are multiple commands (pipes)
                    if (commands->next)
                        handle_pipes(commands, env);  // Add this function call for pipe handling
                    else
                    {
                        // Single command execution
                        execute_single_cmd(commands, env);
                    }
                    
                    free_command_list(commands);
                }
                free_tokens(tokens);
            }
            add_history(line);
        }
        printf("g_exit_status: %d\n", g_exit_status);
        free(line);
    }
}

int main(int argc, char **argv, char **env)
{
    (void)argc;
    (void)argv;
    
    // Initialize signal handlers
    all_signals();
    // Initialize shell with environment
    init_shell(env);
    
    return g_exit_status;
}
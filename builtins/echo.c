#include "../minishell.h"

int is_n_option(char *arg)
{
    int i = 0;
    if (arg[i] == '-' && arg[i + 1] == 'n')
        i += 2;
    else
        return 0;
    while (arg[i]) {
        if (arg[i] != 'n')
            return 0;
        i++;
    }
    return 1;
}

int first_non_option(char **env)
{
    int i = 1; 
    while (env[i] && is_n_option(env[i]))
        i++;
    return i;
}

int echo (t_command *cmd, char **env)
{
    (void)env;
    int i = first_non_option(cmd->args);
    while (cmd->args[i])
    {
        printf("%s", cmd->args[i]);
        if (cmd->args[i + 1])
            printf(" ");
        i++;
    }
    if (first_non_option(cmd->args) == 1)
    {
        printf("\n");
        g_vars.exit_status = 0;
    }
    return 0;
}
#include "../minishell.h"

int is_n_option(char *arg)
{
    int i = 0;
    if (arg && arg[i] == '-' && arg[i + 1] == 'n')
        i += 2;
         else if(!arg)
          return 2;
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
    if (is_n_option(cmd->args[i]) == 2)
        return 0;
    while (cmd->args[i])
    {
        ft_putstr_fd(cmd->args[i], 1);
        if (cmd->args[i + 1])
            ft_putstr_fd(" ", 1);
        i++;
    }
    if (!is_n_option(cmd->args[1]))
        ft_putstr_fd("\n", 1);
    return 0;

}
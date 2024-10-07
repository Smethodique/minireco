#include "../minishell.h"


int pwd(t_command *cmd, char **env)
{
    (void)cmd;
    (void)env;
    char    max_path[1024];

    if(getcwd(max_path, 1024) == NULL)
        return (1);
    printf("%s\n", max_path);
    return (0);
}
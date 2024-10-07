#include "../minishell.h"

void    execute_builtin(t_command *cmd, char **environment, int index)
{
	if (index == 0)
		cd(cmd, environment);
	else if (index == 1)
		env(cmd, environment);
	else if (index == 2)
		export(cmd, environment);
	else if (index == 3)
		unset(cmd, environment);
	else if (index == 4)
		echo(cmd, environment);
	else if (index == 5)
		pwd(cmd, environment);
}

int is_builtin(t_command *cmd)
{
    char *built_in[] = {
        "cd", "env", "export", "unset", "echo", "pwd"
    };
    int num_builtins = sizeof(built_in) / sizeof(built_in[0]);
    int i;

    for (i = 0; i < num_builtins; i++)
    {
        if (ft_strncmp(cmd->args[0], built_in[i], ft_strlen(built_in[i]) + 1) == 0)
        {
            return i;
        }
    }
    return -1; // Return -1 to indicate that the command is not a built-in
}
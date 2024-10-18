#include "../minishell.h"


void ft_exit(t_command *cmd, char **env)
{
    (void)env;
    int i = 0;
    int status = 0;
    if (cmd->arg_count > 2)
    {
        ft_putstr_fd("exit\n", 2);
        ft_putstr_fd("minishell: exit: too many arguments\n", 2);
        g_vars.exit_status = 1;
        return;
    }
    if (cmd->arg_count == 2)
    {
        while (cmd->args[1][i])
        {
            if (!ft_isdigit(cmd->args[1][i]))
            {
                ft_putstr_fd("exit\n", 2);
                ft_putstr_fd("minishell: exit: ", 2);
                ft_putstr_fd(cmd->args[1], 2);
                ft_putstr_fd(": numeric argument required\n", 2);
                g_vars.exit_status = 255;
                return;
            }
            i++;
        }
        status = ft_atoi(cmd->args[1]);
        g_vars.exit_status = status;
    }
    ft_putstr_fd("exit\n", 2);
    exit(g_vars.exit_status);
}

void    execute_builtin(t_command *cmd, char **environment, int index)
{

	if (index == 0)
		cd(cmd, environment);
	else if (index == 1)
		env(cmd);
	else if (index == 2)
		export(cmd);
	else if (index == 3)
		unset(cmd, environment);
	else if (index == 4)
		echo(cmd, environment);
	else if (index == 5)
		pwd(cmd, environment);
    else if(index == 6)
       ft_exit(cmd, environment);   
}

int is_builtin(t_command *cmd)
{
    char *built_in[] = {
        "cd", "env", "export", "unset", "echo", "pwd" , "exit"
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
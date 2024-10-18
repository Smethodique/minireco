#include "../minishell.h"

void    print_export(char *env)
{
    int i = 0;
    printf("declare -x ");
    if (ft_strchr(env, '='))
    {
        while (env[i]!= '=')
            printf("%c", env[i++]);
        printf("=\"");
        i++;
        printf("%s\"\n", (env + i));
    }
    else
        printf("%s\n", env);
}
   
int check_env(char *cmd, char **env)
{
    int i = 0;
    if (cmd == NULL)
        return 0;
    while (env[i])
    {
        if (!ft_strncmp(env[i], cmd, length(env[i])))
            return (i);
        else
            i++;
    }
    return 0;
}
void add_to_envp(char ***env, char *new_var)
{
    int len = double_pointer_len(*env);
    char **new_env = malloc(sizeof(char *) * (len + 2));
    if (new_env)
    {
        int i = 0;
        while (i < len)
        {
            new_env[i] = (*env)[i];
            i++;
        }
        new_env[i] = ft_strdup(new_var);
        new_env[i + 1] = NULL;
        *env = new_env;
    }
}
void export_helper(char *cmd, char ***env, int len)
{
    int x;
    char *new_var;
      (void)len;

    if ((x = check_env(cmd, *env)) && ft_strchr(cmd, '=') 
        && (length(cmd) == length((*env)[x])))
    {
        free((*env)[x]);
        (*env)[x] = ft_strdup(cmd);
    }
    else if (!check_env(cmd, *env))
    {
        new_var = ft_strdup(cmd);
        add_to_envp(env, new_var);
        free(new_var);
    }
}


int check_export(char *cmd)
{
    int  i = 1;
    if (cmd[0] != '_' && !ft_isalpha(cmd[0]))
    {
        ft_putstr_fd("Minishell: not a valid identifier\n", 2);
        return (0);
    }
    while (i < (int)length(cmd))
    {
        if (cmd[i] != '_' && !ft_isalnum(cmd[i]))
        {
            ft_putstr_fd("Minishell: not a valid identifier\n", 2);
            return (0);
        }
        i++;
    }
    if (ft_strncmp(cmd, " ", 2)== 0)
    {
        ft_putstr_fd("Minishell: not a valid identifier\n", 2);
        return (0);
    }   
    return (1);
}
void export(t_command *cmd)
{
    int i;
    int len;

    if (cmd->args[1])
    {
        i = 1;
        while (cmd->args[i])
        {
            if (check_export(cmd->args[i]) == 1)
            {
                len = 0;
                while (g_vars.env[len])
                    len++;
                export_helper(cmd->args[i], &g_vars.env, len);
            }
            i++;
        }        
    }
    else
    {
        i = -1;
        while (g_vars.env[++i])
            print_export(g_vars.env[i]);
    }
}

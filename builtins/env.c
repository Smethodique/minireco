#include "../minishell.h"

static int	*exit_code(void)
{
    static int	c;

    return (&c);
}

int	ft_getter(void)
{
    int	*i;

    i = exit_code();
    return (*i);
}

void	ft_setter(int value)
{
    int	*i;

    i = exit_code();
    *i = value;
}

int	double_pointer_len(char **str)
{
    int	i;

    i = 0;
    if (!str)
        return (0);
    while (str[i])
        i++;
    return (i);
}

void increment_shlvl(char **env)
{
    int i = 0;
    while (env[i])
    {
        if (ft_strncmp(env[i], "SHLVL=", 6) == 0)
        {
            int shlvl = atoi(env[i] + 6);
            shlvl++;
            char *new_shlvl = malloc(20); // Allocate enough space for "SHLVL=" and the new value
            if (new_shlvl)
            {
                sprintf(new_shlvl, "SHLVL=%d", shlvl);
                env[i] = new_shlvl;
            }
            return;
        }
        i++;
    }
}

void env(t_command *cmd, char **env)
{
    int i = 0;

    if (cmd->args && double_pointer_len(cmd->args) != 1) 
    {
        ft_putstr_fd("minishell: env: too many arguments\n", 2);
        ft_setter(1);
        return;
    }

    ft_setter(0);
    increment_shlvl(env); // Increment SHLVL before printing the environment
    while (env[i])
    {
        if (ft_strchr(env[i], '='))
            printf("%s\n", env[i]);
        i++;
    }
}

size_t length(char *s)
{
    size_t i = 0;
    while (s[i] && s[i] != '=')
    {
        i++;
    }
    return i;
}

int check_dup_env(char *cmd, char **env)
{
    int i = 0;
    while (env[i])
    {
        if (!ft_strncmp(env[i], cmd, length(env[i])))
            return (i);
        else
            i++;
    }
    return 0;
}

void unset_helper(char *cmd, char **env, int len)
{
    int checker;
    int i;

    checker = check_dup_env(cmd, env);
    if (checker && (length(env[checker]) == length(cmd)))
    {
        i = checker;
        while (i < len - 1)
        {
            env[i] = env[i + 1];
            i++;
        }
        env[len - 1] = NULL;
    }
}

void unset(t_command *cmd, char **env)
{
    int i = 1;
    int len;
    if (cmd->args[1])
    {
        while (cmd->args[i])
        {
            len = 0;
            while (env[len])
                len++;
            unset_helper(cmd->args[i], env, len);
            i++;
        }
    }
}
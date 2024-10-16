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
void add_to_env(char ***env, char *new_var)
{
    int len = double_pointer_len(*env);
    char **new_env = malloc(sizeof(char *) * (len + 2)); // Allocate space for the new variable and the NULL terminator
    if (new_env)
    {
        int i = 0;
        while (i < len)
        {
            new_env[i] = (*env)[i];
            i++;
        }
        new_env[i] = new_var;
        new_env[i + 1] = NULL;
        free(*env);
        *env = new_env;
    }
}

void free_env(char **env)
{
    int i = 0;
    while (env[i])
    {
        free(env[i]);
        i++;
    }
    free(env);
}


void env(t_command *cmd)
{
    int i = 1;
    char **new_env = NULL;
    int empty_env = 0;

    // Check for -i option
    if (cmd->args && cmd->args[1] && strcmp(cmd->args[1], "-i") == 0) {
        empty_env = 1;
        i++;
    }

    // Create new environment if -i is present or if original env is empty
    if (empty_env || g_vars.env == NULL || g_vars.env[0] == NULL) {
        new_env = create_env();
    } else {
        new_env = g_vars.env;
    }

    // Handle additional variable assignments
    while (cmd->args[i] && strchr(cmd->args[i], '=')) {
        add_to_env(&new_env, cmd->args[i]);
        i++;
    }

    // Update global environment
    g_vars.env = new_env;

    // If there's a command to execute
    if (cmd->args[i]) {
        execute_cmd(&cmd->args[i]);
    } else {
        // Print the environment
        int j = 0;
        while (g_vars.env[j]) {
            printf("%s\n", g_vars.env[j]);
            j++;
        }
    }

    // Clean up if we created a new environment
    if (empty_env || g_vars.env == NULL || g_vars.env[0] == NULL) {
        free_env(new_env);
    }

    ft_setter(0);
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
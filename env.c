
size_t length(char *s)
{
    size_t i = 0;
    while (s[i] && s[i] != '=')
    {
        i++;
    }
    return i;
}

void unset_helper(char *cmd, char **env, int len)
{
    int checker;
    char **env1;
    int i;
    int j;

    checker = 0;
    env1 = NULL;
    checker = check_dup_env(cmd, env);
    if (checker && (length(env[checker]) == length(cmd)))
    {
        env1 = malloc(sizeof(char*) * (len));
        i = 0;
        j = 0;
        while (i < len)
        {
            if (i >= checker && i != (len - 1))
            {
                j++;
                env1[i] = ft_strdup(env[j]);
            }
            else if (i == (len - 1))
                env1[i] = NULL;
            else
                env1[i] = ft_strdup(env[j++]);
            i++;
        }       
        i = -1;
        while (env1[++i])
            env[i] = ft_strdup(env1[i]);
        env[i] = NULL;
        ft_free(env1);
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

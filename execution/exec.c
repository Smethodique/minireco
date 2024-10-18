#include "../minishell.h"

char **ft_free(char **str)
{
    int i;

    i = 0;
    while (str[i])
    {
        free(str[i]);
        i++;
    }
    free(str);
    return (NULL);
}


char *check_path(char **cmd, char **path)
{
    int i;
    char *new_cmd;
    char *new_path;

    i = 0;
    while (path[i])
    {
        new_cmd = ft_strjoin("/", cmd[0]);
        new_path = ft_strjoin(path[i], new_cmd);
        free(new_cmd);
        if (access(new_path, F_OK) == 0)
        {
            ft_free(path);
            return (new_path);
        }
        free(new_path);
        i++;
    }
    ft_free(path);
    return (ft_strdup(cmd[0]));
}

char *get_path(char **cmd)
{
    int i;
    char **path;
    char *env_path = NULL;
    i = 0;
    while (g_vars.env[i])
    {
        if (ft_strncmp(g_vars.env[i], "PATH=", 5) == 0)
        {
            env_path = g_vars.env[i] + 5;
            break;
        }
        i++;
    }
    if (env_path == NULL || *env_path == '\0')
            return ft_strdup(cmd[0]);
    path = ft_splitD(env_path, ":");
    return (check_path(cmd, path));
}

void execute_cmd(char **cmd)
{
    pid_t pid;
    int status;
    char *path;

    pid = fork();
    if (pid == -1)
    {
        perror("minishell: fork failed");
        return;
    }
    else if (pid == 0)
    {
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);

        path = get_path(cmd);
        if (path == NULL)
        {
            if (execve(cmd[0], cmd, g_vars.env) == -1)
            {
                ft_putstr_fd("minishell: command not found: ", 2);
                ft_putstr_fd(cmd[0], 2);
                ft_putstr_fd("\n", 2);
                exit(127);
            }
        }
        else if (execve(path, cmd, g_vars.env) == -1)
        {
            free(path);
            perror("minishell: execution failed");
            exit(126);
        }
    }
    else 
    {
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            g_vars.exit_status = WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
            g_vars.exit_status = 128 + WTERMSIG(status);
    }
}

void exec(t_command *cmd)
{
    char **args;
    int i;

    i = 0;
    args = malloc(sizeof(char *) * (double_pointer_len(cmd->args) + 1));
    if (!args)
        return;
    while (cmd->args[i])
    {
        args[i] = ft_strdup(cmd->args[i]);
        i++;
    }
    args[i] = NULL;
    execute_cmd(args);
    ft_free(args);
}

void execute_single_cmd(t_command *cmd, char **env)
{
    int cmd_type;

    if (cmd == NULL || cmd->args == NULL || cmd->args[0] == NULL)
        return;
     all_signals();
    cmd_type = is_builtin(cmd);
    if (cmd->redirections)
        ft_redict(cmd, env);
    else
    {
        if (cmd_type == -1)
            exec(cmd);
        else
            execute_builtin(cmd, env, cmd_type);
    }
}
#include "../minishell.h"


void    exec_in_child(t_command *cmd, char **env)
{
        cmd->args[0] = get_path(cmd->args, env);
        if (execve(cmd->args[0], cmd->args, env) == -1)
        {
                perror("minishell: execution failed");
                exit(127);
        }
}

void    redic_builtin(t_command *cmd, char **env)
{
        int in = dup(STDIN_FILENO);
        int out = dup(STDOUT_FILENO);
        int new_in = get_in(cmd, STDIN_FILENO);
        int new_out = get_out(cmd, STDOUT_FILENO);

        if (new_in == -1 || new_out == -1)
                return ;
        dup2(new_in, STDIN_FILENO);
        dup2(new_out, STDOUT_FILENO);
        execute_builtin(cmd, env, is_builtin(cmd));
        dup2(in, STDIN_FILENO);
        dup2(out, STDOUT_FILENO);
        close(in);
        close(out);
        if (new_in != STDIN_FILENO)
                close(new_in);
        if (new_out != STDOUT_FILENO)
                close(new_out);
}

void    setup_redirection(t_command *cmd)
{
        int in = get_in(cmd, STDIN_FILENO);
        int out = get_out(cmd, STDOUT_FILENO);

        if (in == -1 || out == -1)
        {
                perror("minishell");
                exit(1);
        }

        if (in != STDIN_FILENO)
        {
                dup2(in, STDIN_FILENO);
                close(in);
        }
        if (out != STDOUT_FILENO)
        {
                dup2(out, STDOUT_FILENO);
                close(out);
        }
}

void    redic_not_builtin(t_command *cmd, char **env)
{
        pid_t pid = fork();

        if (pid == 0)
        {
                signal(SIGINT, SIG_DFL);
                signal(SIGQUIT, SIG_DFL);

                setup_redirection(cmd);

                exec_in_child(cmd, env);
        }
        else if (pid > 0)
        {
                int status;
                waitpid(pid, &status, 0);
                if (WIFEXITED(status))
                        g_vars.exit_status = WEXITSTATUS(status);
                else if (WIFSIGNALED(status))
                        g_vars.exit_status = 128 + WTERMSIG(status);
        }
        else
        {
                perror("minishell: fork failed");
        }
}

void    ft_redict(t_command *cmd, char **env)
{
        if (is_builtin(cmd) == NOT_BUILT_IN)
                redic_not_builtin(cmd, env);
        else
                redic_builtin(cmd, env);
}

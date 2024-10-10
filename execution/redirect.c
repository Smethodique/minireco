#include "../minishell.h"

void exec_in_child(t_command *cmd, char **env)
{
	cmd->args[0] = get_path(cmd->args, env); 
	if (execve(cmd->args[0], cmd->args, env) == -1)
	{
		perror("minishell wst el exec d pip");
		exit(127);
	}
}

void redic_builtin(t_command *cmd, char **env)
{
	int in = 0;
	int out = 1;
	int in_fd = dup(STDIN_FILENO);
	int out_fd = dup(STDOUT_FILENO);
	in = get_in(cmd, in);
	out = get_out(cmd, out);
	if (in == -1 || out == -1)
	{
		perror("minishell");
		exit(1);
	}
	dup_in_out(in, out);
	execute_builtin(cmd, env, is_builtin(cmd));
	restore_fd(in, out, in_fd, out_fd);
	close(in);
	close(out);
}

void redic_not_builtin(t_command *cmd, char **env)
{
    int in = 0;
    int out = 1;
    int pid = fork();
    
    if (pid == 0)
    {
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        
        // Get input/output redirections
        in = get_in(cmd, in);
        out = get_out(cmd, out);
        
        if (in == -1 || out == -1)
        {
            perror("minishell");
            exit(1);
        }
        
        // Set up redirections before executing
        if (in != 0)
        {
            dup2(in, STDIN_FILENO);
            close(in);
        }
        if (out != 1)
        {
            dup2(out, STDOUT_FILENO);
            close(out);
        }
        
        exec_in_child(cmd, env);
        exit(1);
    }
    else 
    {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
             g_vars.exit_status = WEXITSTATUS(status);
        else
             g_vars.exit_status = 128 + WTERMSIG(status);
    }
}

void ft_redict(t_command *cmd, char **env)
{
	if (is_builtin(cmd) == NOT_BUILT_IN)
		redic_not_builtin(cmd, env);
	else
		redic_builtin(cmd, env);
}
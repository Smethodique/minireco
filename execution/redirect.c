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
		signal(2, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		in = get_in(cmd, in);
		out = get_out(cmd, out);
		if (in == -1 || out == -1)
		{
			perror("minishell");
			exit(1);
		}
		dup_in_out(in, out);
		exec_in_child(cmd, env);
		close(in);
		close(out);
	}
}

void ft_redict(t_command *cmd, char **env)
{
	if (is_builtin(cmd) == NOT_BUILT_IN)
		redic_not_builtin(cmd, env);
	else
		redic_builtin(cmd, env);
}
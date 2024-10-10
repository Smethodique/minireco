#include "../minishell.h"

#include "../minishell.h"

// Add this to your header file if not already present
void setup_child_signals(void);

void setup_child_signals(void)
{
    signal(SIGINT, SIG_DFL);   // Reset to default SIGINT handler
    signal(SIGQUIT, SIG_DFL);  // Reset to default SIGQUIT handler
    signal(SIGTSTP, SIG_DFL);  // Reset to default SIGTSTP handler
}

pid_t execute_piped_command(t_command *cmd, int in_fd, int out_fd, char **env)
{
    pid_t pid;

    pid = fork();
    if (pid == -1)
    {
        perror("fork failed");
        exit(1);
    }
    if (pid == 0)
    {
        // Setup signal handlers for child process
        setup_child_signals();
        
        // Set up input redirection
        if (in_fd != STDIN_FILENO)
        {
            dup2(in_fd, STDIN_FILENO);
            close(in_fd);
        }
        
        // Set up output redirection
        if (out_fd != STDOUT_FILENO)
        {
            dup2(out_fd, STDOUT_FILENO);
            close(out_fd);
        }

        // Close all other file descriptors
        int fd;
        for (fd = 3; fd < 256; fd++)  // Close any other open file descriptors
        {
            if (fd != in_fd && fd != out_fd)
                close(fd);
        }

        // Handle command redirections if any
        if (cmd->redirections)
        {
            int red_in = get_in(cmd, STDIN_FILENO);
            int red_out = get_out(cmd, STDOUT_FILENO);
            if (red_in != STDIN_FILENO)
            {
                dup2(red_in, STDIN_FILENO);
                close(red_in);
            }
            if (red_out != STDOUT_FILENO)
            {
                dup2(red_out, STDOUT_FILENO);
                close(red_out);
            }
        }

        // Execute the command
        if (is_builtin(cmd) != NOT_BUILT_IN)
        {
            execute_builtin(cmd, env, is_builtin(cmd));
            exit(0);
        }
        else
        {
            cmd->args[0] = get_path(cmd->args, env);
            if (!cmd->args[0])
            {
                fprintf(stderr, "minishell: command not found: %s\n", cmd->args[0]);
                exit(127);
            }
            if (execve(cmd->args[0], cmd->args, env) == -1)
            {
                perror("minishell: execution failed");
                exit(127);
            }
        }
        exit(0);
    }

    return pid;
}

void handle_pipes(t_command *commands, char **env)
{
    t_command *current;
    int pipe_count = 0;
    int status;
    pid_t *pids;
    int pipes[2][2];
    int i = 0;

    // Count commands
    current = commands;
    while (current)
    {
        pipe_count++;
        current = current->next;
    }
    
    pids = malloc(sizeof(pid_t) * pipe_count);
    if (!pids)
        return;

    current = commands;
    while (current)
    {
        if (current->next && pipe(pipes[i % 2]) == -1)
        {
            perror("pipe failed");
            free(pids);
            return;
        }

        int in_fd = (i > 0) ? pipes[(i + 1) % 2][0] : STDIN_FILENO;
        int out_fd = current->next ? pipes[i % 2][1] : STDOUT_FILENO;

        pids[i] = execute_piped_command(current, in_fd, out_fd, env);

        // Close used pipe ends in parent
        if (i > 0)
            close(pipes[(i + 1) % 2][0]);
        if (current->next)
            close(pipes[i % 2][1]);

        current = current->next;
        i++;
    }

    // Wait for all processes to finish
    for (i = 0; i < pipe_count; i++)
    {
        waitpid(pids[i], &status, 0);
        if (i == pipe_count - 1)
        {
            if (WIFEXITED(status))
                 g_vars.exit_status = WEXITSTATUS(status);
            else if (WIFSIGNALED(status))
                 g_vars.exit_status = 128 + WTERMSIG(status);
        }
    }
    
    free(pids);
}

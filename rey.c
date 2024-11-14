#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex1;
pthread_mutex_t mutex2;

void* thread1_func(void* arg) {
    // Lock mutex1 first
    pthread_mutex_lock(&mutex1);
    printf("Thread 1: locked mutex1\n");

    // Simulate some work
    sleep(1);

    // Now try to lock mutex2 (this will cause deadlock with thread2)
    printf("Thread 1: waiting to lock mutex2\n");
    pthread_mutex_lock(&mutex2);
    printf("Thread 1: locked mutex2\n");

    // Unlock both mutexes after work is done
    pthread_mutex_unlock(&mutex2);
    pthread_mutex_unlock(&mutex1);

    return NULL;
}

void* thread2_func(void* arg) {
    // Lock mutex1 first
    pthread_mutex_lock(&mutex1);
    printf("Thread 2: locked mutex2\n");

    // Simulate some work
    sleep(1);

    // Now try to lock mutex1 (this will cause deadlock with thread1)
    printf("Thread 2: waiting to lock mutex1\n");
    pthread_mutex_lock(&mutex2);
    printf("Thread 2: locked mutex1\n");

    // Unlock both mutexes after work is done
    pthread_mutex_unlock(&mutex1);
    pthread_mutex_unlock(&mutex2);

    return NULL;
}

int main() {
    pthread_t thread1, thread2;

    // Initialize mutexes
    pthread_mutex_init(&mutex1, NULL);
    pthread_mutex_init(&mutex2, NULL);

    // Create two threads
    pthread_create(&thread1, NULL, thread1_func, NULL);
    pthread_create(&thread2, NULL, thread2_func, NULL);

    // Wait for both threads to finish
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    // Destroy mutexes
    pthread_mutex_destroy(&mutex1);
    pthread_mutex_destroy(&mutex2);

    return 0;
}






int	handle_redirection(t_redirection *redir, int fd_in)
{
	int		new_fd;
	char	*expanded;

	new_fd = fd_in;
	if (redir->type == INPUT || redir->type == HEREDOC)
	{
		expanded = ft_strdup(redir->filename);
		if (redir->type == INPUT)
		{
			if (check_file(expanded, O_RDONLY) == -1)
			{
				g_vars.exit_status = 1;
				free(expanded);
				return (-1);
			}
		}
		new_fd = open(expanded, O_RDONLY);
		if (new_fd < 0)
		{
			free(expanded);
			return (-1);
		}
		free(expanded);
	}
	return (new_fd);
}

int	get_in(t_redirection *redir, int fd_in)
{
	t_redirection	*current;
	int				new_fd;

	current = redir;
	new_fd = fd_in;
	while (current)
	{
		new_fd = handle_redirection(current, new_fd);
		if (new_fd == -1)
			return (-1);
		current = current->next;
	}
	return (new_fd);
}

#include "../minishell.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <wordexp.h>

int handle_output_redirection(t_redirection *red, int *heredoc_fd)
{
    int new_fd = -1;
    char *expanded;

    while (red)
    {
        if (red->type == OUTPUT || red->type == APPEND)
        {
            expanded = expand_variables(ft_strdup(red->filename));
            if (ft_strlen(expanded) == 0 || check_directory(get_directory_path(expanded), 1) == -1)
            {
                free(expanded);
                if (*heredoc_fd != -1)
                    close(*heredoc_fd);
                return (ft_putstr_fd("ambiguous redirect\n", 2), -1);
            }
            if (red->type == OUTPUT)
                new_fd = open(expanded, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            else if (red->type == APPEND)
                new_fd = open(expanded, O_WRONLY | O_CREAT | O_APPEND, 0644);
            free(expanded);
        }
        else if (red->type == HEREDOC)
        {
            *heredoc_fd = get_in(red, 0); // Use 0 for stdin
        }
        red = red->next;
    }
    return new_fd;
}

int get_out(t_command *cmd, int fd_out)
{
    t_redirection *red;
    int new_fd = fd_out;
    int heredoc_fd = -1;

    red = cmd->redirections;
    new_fd = handle_output_redirection(red, &heredoc_fd);
    if (new_fd == -1)
        return -1;

 if (red->output_fd != -1)
	{
		while ((red->bytes_read = read(heredoc_fd, red->buffer , 4096)) > 0)
		write(red->output_fd, red->buffer, red->bytes_read);
		close(heredoc_fd);
		close(red->output_fd);
		return (new_fd);
	}

 	dup2(red->output_fd, fd_out);
	close(red->output_fd);
	return (new_fd);
}


int	get_out(t_command *cmd, int fd_out)
{
	t_redirection	*red;
	int				new_fd;
	char			*expanded;

	red = cmd->redirections;
	new_fd = fd_out;
	while (red)
	{
		if (red->type == OUTPUT || red->type == APPEND)
		{
			expanded = expand_variables(ft_strdup(red->filename));
			 if (ft_strlen(expanded) == 0 || check_directory(get_directory_path(expanded), 1) == -1)
			 	return (ft_putstr_fd("ambiguous redirect\n", 2), -1);
			if (red->type == OUTPUT)
				new_fd = open(expanded, O_WRONLY | O_CREAT | O_TRUNC, P);
			else
				new_fd = open(expanded, O_WRONLY | O_CREAT | O_APPEND, P);
			dup2(new_fd, fd_out);
		}
		else if (red->type == INPUT || red->type == HEREDOC)
			if (get_in(red, STDIN_FILENO) < 0)
				return (new_fd);
		red = red->next;
	}
	return (new_fd);
}
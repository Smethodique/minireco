#include "../minishell.h"

void dup_in_out(int in, int out)
{
    if (in)
    {
        dup2(in, STDIN_FILENO);
        close(in);
    }
    if (out != 1)
    {
        dup2(out, STDOUT_FILENO);
        close(out);
    }
}

void restore_fd(int in, int out, int new_in, int new_out)
{
    if (in)
    {
        dup2(new_in, STDIN_FILENO);
        close(new_in);
    }
    if (out != 1)
    {
        dup2(new_out, STDOUT_FILENO);
        close(new_out);
    }
}

int get_in(t_command *tmp, int fd_in)
{
    t_redirection *tmp1;
    
    tmp1 = tmp->redirections;
    while (tmp1)
    {
        if (tmp1->type == INPUT && tmp1->filename)  // Changed from '<' to INPUT
            fd_in = open(tmp1->filename, O_RDONLY);
        else if (tmp1->type == HEREDOC)  // Changed from checking delimiter to HEREDOC type
        {
            char *temp_filename = ft_strjoin("minishell_", ft_itoa(0));
            int fd = open(temp_filename, O_RDWR);
            free(temp_filename);
            if (fd != -1)
                fd_in = fd;
        }
        tmp1 = tmp1->next;
    }
    return fd_in;
}

int get_out(t_command *tmp, int fd_out)
{
    t_redirection *tmp1;
    
    tmp1 = tmp->redirections;
    while (tmp1)
    {
        if (tmp1->type == OUTPUT)  // Changed from '>' to OUTPUT
            fd_out = open(tmp1->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        else if (tmp1->type == APPEND)  // Added APPEND handling
            fd_out = open(tmp1->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
        tmp1 = tmp1->next;
    }
    return fd_out;
}

int get_in_v2(t_command *tmp, int fd_in, int index)
{
    t_redirection *tmp1;
    int fd;
    char *temp_filename;
    
    tmp1 = tmp->redirections;
    while (tmp1)
    {
        if (tmp1->type == INPUT && !tmp1->filename)
            fd_in = open(tmp1->filename, O_RDONLY);
        else if (tmp1->type == HEREDOC)
        {
            temp_filename = ft_strjoin("minishell_", ft_itoa(index));
            fd = open(temp_filename, O_RDWR);
            free(temp_filename);
            if (fd != -1)
                fd_in = fd;
        }
        tmp1 = tmp1->next;
    }
    
    if (fd_in != 0)  // Only unlink if we opened a heredoc file
    {
        temp_filename = ft_strjoin("minishell_", ft_itoa(index));
        unlink(temp_filename);
        free(temp_filename);
    }
    return fd_in;
}
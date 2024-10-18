#include "../minishell.h"
int get_in(t_command *cmd, int fd_in)
{
    t_redirection *redir = cmd->redirections;
    int new_fd = fd_in;
    
    while (redir)
    {
        if (redir->type == INPUT)
        {
            if (new_fd != fd_in)
                close(new_fd);
            new_fd = open(redir->filename, O_RDONLY);
            if (new_fd == -1)
                return (ft_putstr_fd("minishell: ", 2),perror(redir->filename),-1);
        }
        else if (redir->type == HEREDOC)
        {
            if (new_fd != fd_in)
                close(new_fd);
            new_fd = open(redir->filename, O_RDONLY);
            if (new_fd == -1)
                return (ft_putstr_fd("minishell: cannot open heredoc temp file\n", 2),-1);
        }
        redir = redir->next;
    }
    return new_fd;
}

int get_out(t_command *cmd, int fd_out)
{
    t_redirection *redir = cmd->redirections;
    int new_fd = fd_out;
    
    while (redir)
    {
        if (redir->type == OUTPUT)
        {
            if (new_fd != fd_out)
                close(new_fd);
            new_fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (new_fd == -1)
            return (ft_putstr_fd("minishell: ", 2),perror(redir->filename),-1);
        }
        else if (redir->type == APPEND)
        {
            if (new_fd != fd_out)
                close(new_fd);
            new_fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (new_fd == -1)
            return (ft_putstr_fd("minishell: ", 2),perror(redir->filename),-1);
        }
        redir = redir->next;
    }
    return new_fd;
}

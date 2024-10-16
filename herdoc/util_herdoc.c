#include "../minishell.h" // Include this header for O_RDWR, O_CREAT, O_EXCL
#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>

extern t_global_vars g_vars;  // Declare the global variable

void sigint_handlerh(int signum)
{
    (void)signum;
    g_vars.heredoc_interrupted = 1;  // Set flag to indicate interruption
    write(1, "\n", 1);  // Print a newline to avoid terminal corruption
    rl_replace_line("", 0);  // Clear the current input line
    rl_done = 1;  // Force readline to return immediately
}

void init_heredoc(t_heredoc *hd, const char *delimiter, int expand_vars)
{
    hd->delimiter = delimiter;
    hd->unquoted_delimiter = remove_quotes((char *)delimiter);
    hd->is_quoted = (ft_strcmp(delimiter, hd->unquoted_delimiter) != 0);
    hd->content = NULL;
    hd->content_size = 0;
    hd->content_capacity = 0;
    hd->expand_vars = expand_vars;
}

int process_line(t_heredoc *hd)
{
    hd->processed_line = hd->line;
    if (hd->expand_vars && !hd->is_quoted)
    {
        hd->processed_line = expand_env_vars(hd->line);
        if (!hd->processed_line)
        {
            free(hd->line);
            free(hd->content);
            free(hd->unquoted_delimiter);
            return (0);
        }
    }
    hd->line_len = ft_strlen(hd->processed_line);
    return (1);
}

int realloc_content(t_heredoc *hd)
{
    char *new_content;
    size_t new_size;

    new_size = hd->content_size + hd->line_len + 2;
    if (new_size > hd->content_capacity)
    {
        hd->content_capacity = new_size * 2;
        new_content = ft_realloc(hd->content, hd->content_capacity);
        if (!new_content)
        {
            free(hd->line);
            if (hd->expand_vars && hd->processed_line != hd->line)
                free(hd->processed_line);
            free(hd->content);
            free(hd->unquoted_delimiter);
            return (0);
        }
        hd->content = new_content;
    }
    return (1);
}

char *handle_heredoc(const char *delimiter, int expand_vars)
{
    t_heredoc hd;
    void (*prev_handler)(int);  // To store the old signal handler

    g_vars.heredoc_interrupted = 0;
    prev_handler = signal(SIGINT, sigint_handlerh);  // Set the new signal handler

    init_heredoc(&hd, delimiter, expand_vars);
    while (1)
    {
        if (g_vars.heredoc_interrupted)
        {
            free(hd.line);
            break;
        }
        hd.line = readline("> ");
        if (!hd.line)  
            break; // EOF CNTL-D
        if (ft_strcmp(hd.line, hd.unquoted_delimiter) == 0)
        {
            free(hd.line);
            break;
        }
        if (!process_line(&hd) || !realloc_content(&hd))
            break;
        ft_strcpy(hd.content + hd.content_size, hd.processed_line);
        hd.content_size += hd.line_len;
        hd.content[hd.content_size++] = '\n';
        if (hd.expand_vars && hd.processed_line != hd.line)
            free(hd.processed_line);
    }
    if (hd.content)
        hd.content[hd.content_size] = '\0';
    signal(SIGINT, prev_handler);
    if (g_vars.heredoc_interrupted)
        return NULL;
    return hd.content;
}


#include "../minishell.h"


static int check_token(t_token **current, int *command_count, int *redirection_count)
{
    if ((*current)->type == COMMANDE || (*current)->type == ARG)
    {
        (*command_count)++;
        *redirection_count = 0;
    }
    else if ((*current)->type == PIPE)
    {
        if ((*command_count == 0 && *redirection_count == 0) || !(*current)->next)
            return (ft_putstr_fd("Error: Invalid syntax near '|'\n", 2), 0);
        *command_count = 0;
        *redirection_count = 0;
    }
    else if ((*current)->type == OUTPUT || (*current)->type == APPEND
             || (*current)->type == INPUT || (*current)->type == HEREDOC)
    {
        if (!(*current)->next)
            return (ft_putstr_fd("Error: Missing filename after redirection OR delimiter after herdoc \n", 2), 0);
        if ((*current)->type != HEREDOC && (*current)->next->type != FILENAME)
            return (ft_putstr_fd("Error: Missing filename after redirection\n", 2), 0);
        *current = (*current)->next;
        (*redirection_count)++;
    }
    return 1;
}

int validate_syntax(t_token *tokens)
{
    int     command_count;
    int     redirection_count;
    t_token *current;

    command_count = 0;
    redirection_count = 0;
    current = tokens;
    if (current && current->type == PIPE)
        return (ft_putstr_fd("Error: Invalid syntax near '|'\n", 2), 0);
    while (current)
    {
        if (!check_token(&current, &command_count, &redirection_count))
            return 0;
        current = current->next;
    }
    return 1;
}

t_command *parse_tokens(t_token *tokens)
{
    t_parse_context ctx;

    ctx.command_list = NULL;
    ctx.current_command = NULL;
    if (check_heredoc_delim(tokens) == 0 ||!validate_syntax(tokens))
    {
        ft_putstr_fd("Error: Invalid syntax\n", 2);
        return (NULL);
    }
    while (tokens)
    {
        parse_token_one(&ctx, &tokens);
        parse_token_two(&ctx, &tokens);
        parse_token_three(&ctx, &tokens);
        parse_token_four(&ctx, &tokens);
        if (tokens)
        {
            parse_token_five(&ctx, &tokens);
            if (g_vars.heredoc_interrupted)
            {
                g_vars.heredoc_interrupted = 0;
                return ctx.command_list;
            }    
        }
        if (tokens)
            tokens = tokens->next;
    }
    return (ctx.command_list);
}

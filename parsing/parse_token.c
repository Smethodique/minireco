
#include "../minishell.h"


int	validate_syntax(t_token *tokens)
{
	int		command_count;
	int		redirection_count;
	t_token	*current;

	command_count = 0;
	redirection_count = 0;
	current = tokens;
	if (current && current->type == PIPE)
		return(ft_putstr_fd("Error: Invalid syntax near '|'\n", 2), 0);
	while (current)
	{
		if (current->type == COMMANDE || current->type == ARG)
		{
			command_count++;
			redirection_count = 0; 
		}
		else if (current->type == PIPE)
		{
			if (command_count == 0 && redirection_count == 0)
				return (ft_putstr_fd("Error: Invalid syntax near '|'\n", 2), 0);
			command_count = 0;
			redirection_count = 0;
		}
		else if (current->type == OUTPUT || current->type == APPEND
				|| current->type == INPUT || current->type == HEREDOC)
		{
			if (!current->next)
				return (ft_putstr_fd("Error: Missing filename after redirection\n", 2), 0);
			if (current->type != HEREDOC && current->next->type != FILENAME)
				return (ft_putstr_fd("Error: Missing filename after redirection\n", 2), 0);
			current = current->next;
			redirection_count++;
		}
		current = current->next;
	}
	return 1;
}

t_command *parse_tokens(t_token *tokens)
{
    t_parse_context ctx;

    ctx.command_list = NULL;
    ctx.current_command = NULL;
    ctx.status = get_status();
    if (!validate_syntax(tokens))
    {
        return (NULL);
    }
    if (check_heredoc_delim(tokens) == 0)
    {
        ft_putstr_fd("Error: Invalid delimiter after heredoc\n", 2);
        return (NULL);
    }
    while (tokens)
    {
        parse_token_one(&ctx, &tokens);
        parse_token_two(&ctx, &tokens);
        parse_token_three(&ctx, &tokens);
        parse_token_four(&ctx, &tokens);
        parse_token_five(&ctx, &tokens);
        tokens = tokens->next;
    }
    return (ctx.command_list);
}

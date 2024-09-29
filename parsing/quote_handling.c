#include "../minishell.h"

int	handle_quotes(const char *input, int *i, t_quote_info quote_info, t_token **tokens)
{
	int				escaped;
	t_token			*last_token;
	t_token_type	type;

	int start = ++(*i);
	escaped = 0;
	char *quoted, *result = NULL, *final_result = NULL;
	last_token = NULL;
	while (*i < quote_info.len)
	{
		if (input[*i] == '\\' && !escaped && quote_info.quote_char == '"')
		{
			escaped = 1;
			(*i)++;
			continue;
		}
		else if (input[*i] == quote_info.quote_char && !escaped)
		{
			break ;
		}
		else
		{
			escaped = 0;
		}
		(*i)++;
	}
	if (*i >= quote_info.len || input[*i] != quote_info.quote_char)
	{
		ft_putstr_fd("Error: unclosed quote\n", 2);
		return (0);
	}
	quoted = ft_substr(input, start, *i - start);
	if (!quoted)
		return (0);
	if (quote_info.quote_char == '"')
	{
		result = expand_variables(quoted);
		free(quoted);
		final_result = result;
	}
	else
	{
		final_result = quoted;
	}
	if (!final_result)
		return (0);
	type = (*tokens == NULL || (*tokens)->type == PIPE) ? COMMANDE : ARG;
	last_token = new_token(type, final_result);
	add_token(tokens, last_token);
    if (quote_info.quote_char == '"')
{
    free(result);
}
	if (*i + 1 < quote_info.len && input[*i + 1] == ' ')
	{
		if (last_token != NULL)
		{
			last_token->space = 1;
		}
	}
	(*i)++; // Move past the closing quote
	return (1);
}

// Function to handle redirections

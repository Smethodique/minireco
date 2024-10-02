
#include "../minishell.h"

static int	process_quote_content(const char *input, int *i, int quote_len,
		char quote_char)
{
	int	escaped;

	escaped = 0;
	while (*i < quote_len)
	{
		if (input[*i] == '\\' && !escaped && quote_char == '"')
		{
			escaped = 1;
			(*i)++;
			continue ;
		}
		else if (input[*i] == quote_char && !escaped)
		{
			return (1);
		}
		else
		{
			escaped = 0;
		}
		(*i)++;
	}
	ft_putstr_fd("Error: unclosed quote\n", 2);
	    g_exit_status = 2;
	return (0);
}

static char	*extract_and_process_quote(const char *input, int start, int end,
		char quote_char)
{
	char	*quoted;
	char	*result;

	quoted = ft_substr(input, start, end - start);
	if (!quoted)
		return (NULL);
	result = NULL;
	if (quote_char == '"')
	{
		result = expand_variables(quoted);
		free(quoted);
		return (result);
	}
	return (quoted);
}

static int	create_and_add_token(t_token **tokens, char *final_result,
		t_token_type type)
{
	t_token	*last_token;

	last_token = new_token(type, final_result);
	if (!last_token)
		return (0);
	add_token(tokens, last_token);
	return (1);
}
 void   help_norm(const char *input, int *i, t_quote_info quote_info, t_token **tokens) {
    if (*i + 1 < quote_info.len && input[*i + 1] == ' ') 
	{
        t_token *last_token = *tokens;
        while (last_token && last_token->next)
            last_token = last_token->next;
        if (last_token)
            last_token->space = 1;
    }
}

int	handle_quotes(const char *input, int *i, t_quote_info quote_info,
		t_token **tokens)
{
	int				start;
	char			*final_result;
	t_token_type	type;
	// t_token			*last_token;

	start = ++(*i);
	if (!process_quote_content(input, i, quote_info.len, quote_info.quote_char))
		return (0);
	final_result = extract_and_process_quote(input, start, *i,
			quote_info.quote_char);
	if (*tokens == NULL || (*tokens)->type == PIPE)
		type = COMMANDE;
	else if ((*tokens)->type == INPUT || (*tokens)->type == OUTPUT
		|| (*tokens)->type == APPEND )
		type = FILENAME;
	else
		type = ARG;
	if (!create_and_add_token(tokens, final_result, type))
		return (free(final_result), 0);
	if (quote_info.quote_char == '"')
		free(final_result);
	help_norm(input, i, quote_info, tokens);

	(*i)++;
	return (1);
}

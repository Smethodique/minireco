#include "../minishell.h"

static int	handle_quotes_finalize(t_handle_quotes_params *params,
		char *final_result, int start)
{
	t_token_type	type;
	t_token			*last_token;

	last_token = NULL;
	(void)start;
	if (*(params->tokens) == NULL || (*(params->tokens))->type == PIPE)
		type = COMMANDE;
	else
		type = ARG;
	last_token = new_token(type, final_result);
	add_token(params->tokens, last_token);
	if (params->quote_info.quote_char == '"')
		free(final_result);
	if (*(params->i) + 1 < params->quote_info.len && params->input[*(params->i)
		+ 1] == ' ')
	{
		if (last_token != NULL)
			last_token->space = 1;
	}
	(*(params->i))++;
	return (1);
}
static int	handle_quotes_process(t_handle_quotes_params *params, char *quoted,
		int start)
{
	char	*final_result;
	char	*result;

	final_result = NULL;
	if (params->quote_info.quote_char == '"')
	{
		result = expand_variables(quoted);
		free(quoted);
		final_result = result;
	}
	else
		final_result = quoted;
	if (!final_result)
		return (0);
	return (handle_quotes_finalize(params, final_result, start));
}

static int	handle_quotes_core(t_handle_quotes_params *params, int start,
		int escaped)
{
	char	*quoted;
	while (*(params->i) < params->quote_info.len)
	{
		if (params->input[*(params->i)] == '\\' && !escaped
			&& params->quote_info.quote_char == '"')
		{
			escaped = 1;
			(*(params->i))++;
			continue ;
		}
		else if (params->input[*(params->i)] == params->quote_info.quote_char
				&& !escaped)
			break ;
		else
			escaped = 0;
		(*(params->i))++;
	}
	if (*(params->i) >= params->quote_info.len
		|| params->input[*(params->i)] != params->quote_info.quote_char || !calculate_quote_num(params->input, params->quote_info.len, &params->quote_info.len, &params->quote_info.len))
		return (ft_putstr_fd("Error: Unclosed quote\n", 2), 0);
	quoted = ft_substr(params->input, start, *(params->i) - start);
	if (!quoted)
		return (0);
	return (handle_quotes_process(params, quoted, start));
}

int	handle_quotes(const char *input, int *i, t_quote_info quote_info,
		t_token **tokens)
{
	t_handle_quotes_params	params;
	int						start;
	int						escaped;

	params.input = input;
	params.i = i;
	params.quote_info = quote_info;
	params.tokens = tokens;
	start = ++(*i);
	escaped = 0;
	if (!handle_quotes_core(&params, start, escaped))
		return (0);
	return (1);
}



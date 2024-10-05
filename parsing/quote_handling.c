#include "../minishell.h"

#include "../minishell.h"

static int create_and_add_token(t_token **tokens, char *final_result, t_token_type type)
{
    if (!final_result)
        return 0;

    char *cleaned_result =expand_variables(final_result);
    free(final_result);  // Always free final_result

    if (!cleaned_result)
        return 0;

    t_token *last_token = new_token(type, cleaned_result);
    free(cleaned_result);  // Free cleaned_result as new_token makes a copy

    if (!last_token)
        return 0;

    add_token(tokens, last_token);
    return 1;
}

static int process_quote_content(const char *input, int *i, int quote_len,
		char quote_char)
{
	(*i)++;  // Skip the opening quote
	while (*i < quote_len)
	{
		if (input[*i] == quote_char)
		{
			(*i)++;  // Move past the closing quote
			return (1);  // Found matching quote
		}
		else if (input[*i] == '"' || input[*i] == '\'')
		{
			char nested_quote_char = input[*i];
			if (!process_quote_content(input, i, quote_len, nested_quote_char))
				return (0);  // Error in nested quote
		}
		else
		{
			(*i)++;
		}
	}
	ft_putstr_fd("Error: unclosed quote\n", 2);
	g_exit_status = 2;
	return (0);
}

static char *extract_quote(const char *input, int start, int end)
{
	return ft_substr(input, start - 1, end - start + 2);  // Include both quotes
}

int handle_quotes(const char *input, int *i, t_quote_info quote_info,
		t_token **tokens)
{
	int start;
	char *quoted_content;
	t_token_type type;
	
	start = *i + 1;
	if (!process_quote_content(input, i, quote_info.len, quote_info.quote_char))
		return (0);
	quoted_content = extract_quote(input, start, *i - 1);
	if (!quoted_content)
		return (0);
	if (*tokens == NULL || (*tokens)->type == PIPE)
		type = COMMANDE;
	else if ((*tokens)->type == INPUT || (*tokens)->type == OUTPUT
		|| (*tokens)->type == APPEND)
		type = FILENAME;
	else
		type = ARG;
	if (!create_and_add_token(tokens, quoted_content, type))
	{
		free(quoted_content);
		return (0);
	}
	if (*i + 1 < quote_info.len && input[*i + 1] == ' ')
	{
		t_token *last_token = *tokens;
		while (last_token && last_token->next)
			last_token = last_token->next;
		if (last_token)
			last_token->space = 1;
	}

	return (1);
}



#include "../minishell.h"



static void initialize_quote_state(t_quote_state *state)
{
	state->i = 0;
	state->j = 0;
	state->in_double_quotes = 0;
	state->in_single_quotes = 0;
}

static void pro_quotes(const char *str, char *result, int len)
{
	t_quote_state state;

	initialize_quote_state(&state);
	while (state.i < len)
	{
		if (str[state.i] == '"')
		{
			if (!state.in_single_quotes)
				state.in_double_quotes = !state.in_double_quotes;
			else
				result[state.j++] = str[state.i];
		}
		else if (str[state.i] == '\'')
		{
			if (!state.in_double_quotes)
				state.in_single_quotes = !state.in_single_quotes;
			else
				result[state.j++] = str[state.i];
		}
		else
			result[state.j++] = str[state.i];
		state.i++;
	}
	result[state.j] = '\0';
}

char	*remove_quotes(const char *str)
{
	int		len;
	char	*result;

	len = ft_strlen(str);
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	pro_quotes(str, result, len);
	return (result);
}
#include "../minishell.h"

void initialize_state(t_lexer_state *state, const char *input)
{
	state->tokens = NULL;
	state->i = 0;
	state->len = strlen(input);
	state->expect_heredoc_delim = 0;
	state->expect_filename = 0;
}

void update_current_and_next_char(t_lexer_state *state, const char *input)
{
	state->current_char = input[state->i];
	state->next_char = (state->i + 1 < state->len) ? input[state->i + 1] : '\0';
}

int handle_heredoc_cases(t_lexer_state *state, const char *input)
{
	if (state->current_char == '<' && state->next_char == '<')
	{
		handlee_heredoc(&state->i, &state->tokens);
		state->expect_heredoc_delim = 1;
		return 1;
	}
	if (state->expect_heredoc_delim)
	{
		handle_heredoc_delim(input, &state->i, state->len, &state->tokens);
		state->expect_heredoc_delim = 0;
		return 1;
	}
	return 0;
}

int handle_whitespace(t_lexer_state *state)
{
	if (isspace(state->current_char))
	{
		state->i++;
		return 1;
	}
	return 0;
}

int handle_pipe(t_lexer_state *state)
{
	if (state->current_char == '|')
	{
		add_token(&state->tokens, new_token(PIPE, "|"));
		state->i++;
		return 1;
	}
	return 0;
}

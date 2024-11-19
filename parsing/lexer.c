/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakhtou <stakhtou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 17:05:45 by stakhtou          #+#    #+#             */
/*   Updated: 2024/10/24 03:12:27 by stakhtou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_redirection_cases(t_lexer_state *state, const char *input)
{
	(void)input;
	if (state->current_char == '<' || state->current_char == '>')
	{
		handle_redirections(&state->i,
			(t_redirection_chars){state->current_char, state->next_char},
			&state->tokens, &state->expect_filename);
		return (1);
	}
	return (0);
}

int	handle_special_cases(t_lexer_state *state, const char *input)
{
	if (state->expect_filename)
	{
		handle_filename(input, &state->i, state->len, &state->tokens);
		state->expect_filename = 0;
		return (1);
	}
	if (state->current_char == '$')
	{
		handle_env_var(input, &state->i, state->len, &state->tokens);
		return (1);
	}
	return (0);
}

t_token	*tokenize_input(const char *input)
{
	t_lexer_state	state;

	initialize_state(&state, input);
	while (state.i < state.len)
	{
		update_current_and_next_char(&state, input);
		if (handle_heredoc_cases(&state, input))
			continue ;
		if (handle_whitespace(&state))
			continue ;
		if (handle_pipe(&state))
			continue ;
		if (handle_redirection_cases(&state, input))
			continue ;
		if (handle_special_cases(&state, input))
			continue ;
		handle_command_or_argument(input, &state.i, state.len, &state.tokens);
		if (state.i >= state.len)
			return (state.tokens);
		if (!state.tokens)
			return (free_tokens(state.tokens), NULL);
	}
	concatinate(&state.tokens);
	return (state.tokens);
}

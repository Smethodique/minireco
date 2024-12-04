/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helplexer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakhtou <stakhtou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 17:05:45 by stakhtou          #+#    #+#             */
/*   Updated: 2024/10/21 18:42:30 by stakhtou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	initialize_state(t_lexer_state *state, const char *input)
{
	state->tokens = NULL;
	state->i = 0;
	state->len = ft_strlen(input);
	state->expect_heredoc_delim = 0;
	state->expect_filename = 0;
}

void	update_current_and_next_char(t_lexer_state *state, const char *input)
{
	state->current_char = input[state->i];
	if (state->i + 1 < state->len)
	{
		state->next_char = input[state->i + 1];
	}
	else
	{
		state->next_char = '\0';
	}
}

int	handle_heredoc_cases(t_lexer_state *state, const char *input)
{
	if (state->current_char == '<' && state->next_char == '<')
	{
		handlee_heredoc(&state->i, &state->tokens);
		state->expect_heredoc_delim = 1;
		return (1);
	}
	if (state->expect_heredoc_delim)
	{
		handle_heredoc_delim(input, &state->i, state->len, &state->tokens);
		state->expect_heredoc_delim = 0;
		return (1);
	}
	return (0);
}

int	handle_whitespace(t_lexer_state *state)
{
	if (ft_isspace(state->current_char))
	{
		state->i++;
		return (1);
	}
	return (0);
}

int	handle_pipe(t_lexer_state *state)
{
	if (state->current_char == '|')
	{
		add_token(&state->tokens, new_token(PIPE, "|"));
		state->i++;
		return (1);
	}
	return (0);
}

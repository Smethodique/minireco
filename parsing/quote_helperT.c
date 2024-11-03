/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_helperT.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakhtou <stakhtou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 17:05:45 by stakhtou          #+#    #+#             */
/*   Updated: 2024/10/21 18:42:44 by stakhtou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	initialize_quote_state(t_quote_state *state)
{
	state->i = 0;
	state->j = 0;
	state->in_double_quotes = 0;
	state->in_single_quotes = 0;
}

static void	pro_quotes(const char *str, char *result, int len)
{
	t_quote_state	state;

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

static void	concat_tokens(t_token *current, t_token *next)
{
	char	*new_value;

	new_value = malloc(ft_strlen(current->value) + ft_strlen(next->value) + 1);
	if (!new_value)
		return ;
	ft_strcpy(new_value, current->value);
	ft_strcat(new_value, next->value);
	free(current->value);
	current->value = new_value;
	current->next = next->next;
	current->space = next->space;
	free(next->value);
	free(next);
}

void	concatinate(t_token **tokens)
{
	t_token	*current;
	t_token	*next;

	current = *tokens;
	while (current && current->next)
	{
		next = current->next;
		if ((current->type == ARG || current->type == COMMANDE)
			&& (next->type == ARG || next->type == COMMANDE) && !current->space)
		{
			concat_tokens(current, next);
			if (!current->next)
				break ;
		}
		else
			current = current->next;
	}
}

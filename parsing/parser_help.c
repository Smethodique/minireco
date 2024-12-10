/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_help.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iabboudi <iabboudi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 17:05:45 by stakhtou          #+#    #+#             */
/*   Updated: 2024/12/07 21:01:29 by iabboudi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_isalpha(int c)
{
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

int	ft_isalnum(int c)
{
	return (ft_isalpha(c) || ft_isdigit(c));
}

void	handlee_heredoc(int *i, t_token **tokens)
{
	add_token(tokens, new_token(HEREDOC, "<<"));
	*i += 2;
}

void	handle_heredoc_delim(const char *input, int *i, int len,
		t_token **tokens)
{
	int		start;
	char	*delimiter;

	while (*i < len && ft_isspace(input[*i]))
		(*i)++;
	start = *i;
	while (*i < len && !ft_isspace(input[*i]) && input[*i] != '<'
		&& input[*i] != '>' && input[*i] != '|')
		(*i)++;
	if (input[start] == '<' && input[start + 1] == '<')
	{
		handlee_heredoc(i, tokens);
		return ;
	}
	delimiter = ft_strndup(input + start, *i - start);
	if (!delimiter || delimiter[0] == '\0')
	{
		free(delimiter);
		return ;
	}
	add_token(tokens, new_token(DELIMITER, delimiter));
	free(delimiter);
}

void	handle_redirections(int *i, t_redirection_chars chars, t_token **tokens,
		int *expect_filename)
{
	if (chars.current_char == '<')
	{
		*expect_filename = 1;
		add_token(tokens, new_token(INPUT, "<"));
	}
	else if (chars.current_char == '>' && chars.next_char == '>')
	{
		*expect_filename = 1;
		add_token(tokens, new_token(APPEND, ">>"));
		(*i)++;
	}
	else
	{
		*expect_filename = 1;
		add_token(tokens, new_token(OUTPUT, ">"));
	}
	(*i)++;
}

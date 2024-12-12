/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iabboudi <iabboudi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 17:05:45 by stakhtou          #+#    #+#             */
/*   Updated: 2024/12/12 07:59:02 by iabboudi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	check_token(t_token **current, int *command_count,
		int *redirection_count)
{
	if ((*current)->type == COMMANDE || (*current)->type == ARG)
	{
		(*command_count)++;
		*redirection_count = 0;
	}
	else if ((*current)->type == PIPE)
	{
		if ((*command_count == 0 && *redirection_count == 0)
			|| !(*current)->next)
			return (ft_putstr_fd("Error: Invalid syntax near '|'\n", 2), 0);
		*command_count = 0;
		*redirection_count = 0;
	}
	else if ((*current)->type == OUTPUT || (*current)->type == APPEND
		|| (*current)->type == INPUT || (*current)->type == HEREDOC)
	{
		if (!(*current)->next)
			return (0);
		if ((*current)->type != HEREDOC && (*current)->next->type != FILENAME)
			return (ft_putstr_fd("Error: Missing filename  redirection\n", 2),
				0);
		*current = (*current)->next;
		(*redirection_count)++;
	}
	return (1);
}

int	validate_syntax(t_token *tokens)
{
	int		command_count;
	int		redirection_count;
	t_token	*current;

	command_count = 0;
	redirection_count = 0;
	current = tokens;
	if (current && current->type == PIPE)
		return (ft_putstr_fd("Error: Invalid syntax near '|'\n", 2), 0);
	while (current)
	{
		if (!check_token(&current, &command_count, &redirection_count))
			return (0);
		current = current->next;
	}
	return (1);
}

void	all_parse(t_parse_context *ctx, t_token **tokens)
{
	parse_token_one(ctx, tokens);
	parse_token_three(ctx, tokens);
	parse_token_four(ctx, tokens);
}

t_command	*parse_tokens(t_token *tokens)
{
	t_parse_context	ctx;

	ctx.command_list = NULL;
	ctx.current_command = NULL;
	if (check_heredoc_delim(tokens) == 0 || !validate_syntax(tokens))
		return (ft_putstr_fd("Error: Invalid syntax\n", 2),
			g_vars.exit_status = 2, NULL);
	while (tokens)
	{
		all_parse(&ctx, &tokens);
		if (tokens)
		{
			parse_token_five(&ctx, &tokens);
			if (g_vars.heredoc_interrupted)
			{
				free_command_list(ctx.command_list);
				g_vars.exit_status = 130;
				g_vars.heredoc_interrupted = 0;
				return (NULL);
			}
			tokens = tokens->next;
		}
	}
	return (ctx.command_list);
}

void	setup_terminal(void)
{
	struct termios	term;

	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

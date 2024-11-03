/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_com_arg.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakhtou <stakhtou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 17:05:45 by stakhtou          #+#    #+#             */
/*   Updated: 2024/10/21 16:39:32 by stakhtou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	initialize_handle_vars(t_handle_vars *vars, const char *input, int *i,
		t_token **tokens)
{
	(void)input;
	vars->start = *i;
	vars->in_single_quotes = 0;
	vars->in_double_quotes = 0;
	vars->escaped = 0;
	vars->last_token = *tokens;
	while (vars->last_token && vars->last_token->next)
		vars->last_token = vars->last_token->next;
}

void	parse_comm(const char *input, int *i, int len, t_handle_vars *vars)
{
	while (*i < len)
	{
		if (input[*i] == '\'' && !vars->in_double_quotes && !vars->escaped)
			vars->in_single_quotes = !vars->in_single_quotes;
		else if (input[*i] == '"' && !vars->in_single_quotes && !vars->escaped)
			vars->in_double_quotes = !vars->in_double_quotes;
		else if (!vars->in_single_quotes && !vars->in_double_quotes
			&& !vars->escaped && (ft_isspace(input[*i]) || input[*i] == '|'
				|| input[*i] == '<' || input[*i] == '>'))
			break ;
		vars->escaped = 0;
		(*i)++;
	}
}

void	process_value(const char *input, int *i, t_handle_vars *vars,
		t_token **tokens)
{
	vars->value = ft_strndup(input + vars->start, *i - vars->start);
	if (!vars->value)
	{
		ft_putstr_fd("Error: failed to allocate memory\n", 2);
		return ;
	}
	vars->expanded_value = expand_variables(vars->value);
	free(vars->value);
	if (!vars->expanded_value)
		return ;
	vars->final_value = ft_strdup(vars->expanded_value);
	free(vars->expanded_value);
	if (!vars->final_value)
		return ;
	if (*tokens == NULL || vars->last_token->type == PIPE
		|| vars->last_token->type == FILENAME
		|| vars->last_token->type == DELIMITER)
		vars->type = COMMANDE;
	else
		vars->type = ARG;
	vars->new = new_token(vars->type, vars->final_value);
	vars->new->space = (input[*i] == ' ');
	add_token(tokens, vars->new);
	free(vars->final_value);
}

int	detect_unclosed_quotes(const char *str)
{
	int	i;
	int	in_double_quotes;
	int	in_single_quotes;

	i = 0;
	in_double_quotes = 0;
	in_single_quotes = 0;
	while (str && str[i])
	{
		if (str[i] == '"' && !in_single_quotes)
			in_double_quotes = !in_double_quotes;
		else if (str[i] == '\'' && !in_double_quotes)
			in_single_quotes = !in_single_quotes;
		i++;
	}
	if (in_double_quotes || in_single_quotes)
	{
		return (1);
	}
	return (0);
}

void	handle_command_or_argument(const char *input, int *i, int len,
		t_token **tokens)
{
	t_handle_vars	vars;
	int				unclosed_quotes;

	initialize_handle_vars(&vars, input, i, tokens);
	parse_comm(input, i, len, &vars);
	unclosed_quotes = detect_unclosed_quotes(input);
	if (unclosed_quotes == 1 || unclosed_quotes == 2)
	{
		ft_putstr_fd("Error: unclosed quote\n", 2);
		g_vars.exit_status = 2;
		free_tokens(*tokens);
		*tokens = NULL;
		return ;
	}
	process_value(input, i, &vars, tokens);
}

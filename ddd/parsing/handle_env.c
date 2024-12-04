/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakhtou <stakhtou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 17:05:45 by stakhtou          #+#    #+#             */
/*   Updated: 2024/10/21 16:40:26 by stakhtou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_special_env_var(char *env_var, t_token **tokens)
{
	char	*exit_status_str;
	t_token	*new_tok;

	(void)env_var;
	exit_status_str = ft_itoa(g_vars.exit_status);
	new_tok = new_token(ARG, exit_status_str);
	add_token(tokens, new_tok);
	free(exit_status_str);
}

void	handle_regular_env_var(char *env_var, t_token **tokens)
{
	t_env_var_data	data;

	data.env_value = get_env_value(env_var + 1, g_vars.env);
	if (data.env_value)
	{
		if (is_quoted(data.env_value))
			add_token(tokens, new_token(ARG, data.env_value));
		else
		{
			data.split_value = ft_split(data.env_value, " \t");
			data.j = 0;
			while (data.split_value[data.j] != NULL)
			{
				data.new = new_token(ARG, data.split_value[data.j]);
				if (data.split_value[data.j + 1] != NULL)
					data.new->space = 1;
				add_token(tokens, data.new);
				free(data.split_value[data.j]);
				data.j++;
			}
			free(data.split_value);
		}
	}
	else
		add_token(tokens, new_token(ARG, ""));
}

void	handle_env_var_value(char *env_var, t_token **tokens)
{
	if (env_var[1] == '?')
	{
		handle_special_env_var(env_var, tokens);
	}
	else
	{
		handle_regular_env_var(env_var, tokens);
	}
}

void	extract_env_var(const char *input, int *i, int len, char **env_var)
{
	int	start;

	start = (*i)++;
	if (*i < len && input[*i] == '?')
		(*i)++;
	else if (*i < len && (ft_isalnum(input[*i]) || input[*i] == '_'))
	{
		while (*i < len && (ft_isalnum(input[*i]) || input[*i] == '_'))
			(*i)++;
	}
	else
	{
		*env_var = ft_strdup("$");
		return ;
	}
	*env_var = ft_substr(input, start, *i - start);
}

void	handle_env_var(const char *input, int *i, int len, t_token **tokens)
{
	char	*env_var;
	int		space_after_env_var;
	t_token	*last_token;

	extract_env_var(input, i, len, &env_var);
	if (env_var[0] == '$' && env_var[1] == '\0')
	{
		add_token(tokens, new_token(ARG, env_var));
		free(env_var);
		return ;
	}
	space_after_env_var = (*i < len && (input[*i] == ' ' || input[*i] == '\t'));
	handle_env_var_value(env_var, tokens);
	if (space_after_env_var)
	{
		last_token = *tokens;
		while (last_token->next != NULL)
			last_token = last_token->next;
		last_token->space = 1;
	}
	free(env_var);
}

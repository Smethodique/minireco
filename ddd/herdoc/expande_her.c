/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expande_her.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakhtou <stakhtou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 17:05:45 by stakhtou          #+#    #+#             */
/*   Updated: 2024/10/21 03:26:14 by stakhtou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*process_input(t_expv *expv)
{
	char	*temp;

	while (*expv->p)
	{
		if (*expv->p == '$' && (ft_isalnum(*(expv->p + 1)) || *(expv->p
					+ 1) == '_'))
		{
			temp = expand_single_var(&expv->p, expv->result, &expv->result_len,
					&expv->result_cap);
			if (!temp)
				return (free(expv->result), NULL);
			expv->result = temp;
		}
		else
		{
			temp = copy_char_to_result(expv->result, &expv->result_len,
					&expv->result_cap, *expv->p);
			if (!temp)
				return (free(expv->result), NULL);
			expv->result = temp;
			expv->p++;
		}
	}
	expv->result[expv->result_len] = '\0';
	return (expv->result);
}

char	*expand_env_vars(char *input)
{
	t_expv	expv;

	if (!input)
		return (NULL);
	expv.result_len = 0;
	expv.result_cap = ft_strlen(input) * 2;
	expv.result = ft_calloc(expv.result_cap, sizeof(char));
	if (!expv.result)
		return (NULL);
	expv.p = input;
	return (process_input(&expv));
}

char	*expand_single_var(char **p, char *result, size_t *result_len,
		size_t *result_cap)
{
	t_var_expansion	var;

	var.var_start = *p + 1;
	var.var_end = var.var_start;
	while (ft_isalnum(*var.var_end) || *var.var_end == '_')
		var.var_end++;
	var.var_name_len = var.var_end - var.var_start;
	var.var_name = malloc(var.var_name_len + 1);
	ft_strncpy(var.var_name, var.var_start, var.var_name_len);
	var.var_name[var.var_name_len] = '\0';
	var.var_value = get_env_value(var.var_name, g_vars.env);
	if (var.var_value)
	{
		var.value_len = ft_strlen(var.var_value);
		if (*result_len + var.value_len >= *result_cap)
		{
			*result_cap = (*result_len + var.value_len) * 2;
			result = realloc_result_buffer(result, *result_cap);
		}
		ft_strcpy(result + *result_len, var.var_value);
		*result_len += var.value_len;
	}
	*p = var.var_end;
	return (free(var.var_name), result);
}

char	*realloc_result_buffer(char *result, size_t new_cap)
{
	char	*new_result;

	new_result = malloc(new_cap);
	if (!new_result)
	{
		free(result);
		return (NULL);
	}
	ft_strcpy(new_result, result);
	free(result);
	return (new_result);
}

char	*copy_char_to_result(char *result, size_t *result_len,
		size_t *result_cap, char c)
{
	if (*result_len + 1 >= *result_cap)
	{
		*result_cap *= 2;
		result = realloc_result_buffer(result, *result_cap);
		if (!result)
			return (NULL);
	}
	result[(*result_len)++] = c;
	return (result);
}

#include "../minishell.h"

char	*ft_strjoin_char(char *s, char c)
{
	char	*new_str;
	int		len;

	len = ft_strlen(s);
	new_str = (char *)malloc(len + 2);
	if (!new_str)
		return (NULL);
	ft_memcpy(new_str, s, len);
	new_str[len] = c;
	new_str[len + 1] = '\0';
	return (new_str);
}

char *process_quotes(t_expansion *exp)
{
    if (*exp->temp == '\'') // Handle single quotes
    {
        if (exp->in_double_quote == 0) // Only toggle if not inside double quotes
        {
            exp->in_single_quote = !exp->in_single_quote;
        }
        exp->new_result = ft_strjoin_char(exp->result, *exp->temp); // Append the quote itself
    }
    else if (*exp->temp == '"') // Handle double quotes
    {
        if (exp->in_single_quote == 0) // Only toggle if not inside single quotes
        {
            exp->in_double_quote = !exp->in_double_quote;
        }
        exp->new_result = ft_strjoin_char(exp->result, *exp->temp); // Append the quote itself
    }
    else // Handle regular characters
    {
        exp->new_result = ft_strjoin_char(exp->result, *exp->temp);
    }
    free(exp->result);
    exp->result = exp->new_result;
    exp->temp++; // Move past the current character
    return (exp->result);
}



char	*expand_env_variable(t_expansion *exp)
{

	exp->before_env = ft_substr(exp->temp, 0, exp->env_pos - exp->temp);
	exp->new_result = ft_strjoin(exp->result, exp->before_env);
	free(exp->result);
	free(exp->before_env);
	exp->result = exp->new_result;
	exp->env_len = 0;
	while (exp->env_pos[1 + exp->env_len] && (isalnum(exp->env_pos[1 + exp->env_len]) || exp->env_pos[1 + exp->env_len] == '_'))
		exp->env_len++;
	exp->env_name = ft_substr(exp->env_pos + 1, 0, exp->env_len);
	exp->env_value = getenv(exp->env_name);
	free(exp->env_name);
	if (exp->env_value)
	{
		exp->new_result = ft_strjoin(exp->result, exp->env_value);
		free(exp->result);
		exp->result = exp->new_result;
	}
	exp->temp = exp->env_pos + 1 + exp->env_len;
	return (exp->result);
}




char	*expand_variables(const char *str)
{
	t_expansion exp;

	exp.result = ft_strdup("");
	exp.temp = (char *)str;
	while (*exp.temp)
	{
		if (*exp.temp == '\'' || *exp.temp == '"')  
		{
			exp.result = process_quotes(&exp);
			continue;
		}
		if ((exp.in_single_quote || exp.in_double_quote) && (exp.env_pos = strchr(exp.temp, '$')) && exp.env_pos[1] != '\0')
			exp.result = expand_env_variable(&exp);
		else
		{
			exp.new_result = ft_strjoin_char(exp.result, *exp.temp);
			free(exp.result);
			exp.result = exp.new_result;
			exp.temp++;
		}
	}
	exp.final_result = ft_strdup(exp.result);
	free(exp.result);
	exp.unquoted_result = remove_quotes(exp.final_result);
	return(free(exp.final_result), exp.unquoted_result);
}
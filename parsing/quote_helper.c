#include "../minishell.h"
#include <string.h> // Add this line to declare strdup

char	*remove_quotes(const char *str)
{
	int		len;
	char	*result;
	int		j;
	int		in_double_quotes;
	int		in_single_quotes;

	len = ft_strlen(str);
	result = malloc(len + 1);
	j = 0;

	in_double_quotes = 0;
	in_single_quotes = 0;
	if (!result)
		return (NULL);
	int i = 0;
	while (i < len)
	{
		if (str[i] == '"')
		{
			if (!in_single_quotes)
				in_double_quotes = !in_double_quotes;
			else
				result[j++] = str[i];
		}
		else if (str[i] == '\'')
		{
			if (!in_double_quotes)
				in_single_quotes = !in_single_quotes;
			else
				result[j++] = str[i];
		}
		else
			result[j++] = str[i];
		i++;
	}
	result[j] = '\0';
	return (result);
}

int	is_quoted(const char *str)
{
	int	in_single_quotes;
	int	in_double_quotes;

	in_single_quotes = 0;
	in_double_quotes = 0;
	for (int i = 0; str[i] != '\0'; i++)
	{
		if (str[i] == '\'' && !in_double_quotes)
		{
			in_single_quotes = !in_single_quotes;
		}
		else if (str[i] == '"' && !in_single_quotes)
		{
			in_double_quotes = !in_double_quotes;
		}
	}
	return (in_single_quotes || in_double_quotes);
}
char	*remove_enclosing_quotes(char *str)
{
	int	len;

	len = strlen(str);
	if (len >= 2 && ((str[0] == '\'' && str[len - 1] == '\'') || (str[0] == '"'
				&& str[len - 1] == '"')))
	{
		str[len - 1] = '\0';
		return (str + 1);
	}
	return (str);
}

char	*remove_single_quotes(const char *str)
{
	int		len;
	char	*result;
	int		j;
	int		in_single_quotes;

	len = strlen(str);
	result = malloc(len + 1);
	j = 0;
	in_single_quotes = 0;
	for (int i = 0; i < len; i++)
	{
		if (str[i] == '\'')
			in_single_quotes = !in_single_quotes;
		else
			result[j++] = str[i];
	}
	result[j] = '\0';
	if (in_single_quotes)
	{
		free(result);
		printf("Error: unclosed quote\n");
		g_exit_status = 2 ;
		
		return (strdup(str));
	}
	return (result);
}


int	calculate_quote_num(const char *input, int len, int *j, int *p)
{
	*j = 0;
	*p = 0;
	for (int i = 0; i < len; i++)
	{
		if (input[i] == '\'')
		{
			(*j)++;
		}
		else if (input[i] == '"')
		{
			(*p)++;
		}
	}
	if (*j % 2 != 0 || *p % 2 != 0)
	{
		return (0);
	}
	return (1);
}

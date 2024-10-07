#include "../minishell.h"
#include <string.h> // Add this line to declare strdup



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
		printf("minishell: syntax error\n");
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

#include "../minishell.h"

t_token	*tokenize_input(const char *input)
{
	t_token	*tokens;
	int		i;
	int		len;
	int		expect_heredoc_delim;
	int		expect_filename;
	char	current_char;
	char	next_char;

	tokens = NULL;
	i = 0;
	len = strlen(input);
	expect_heredoc_delim = 0;
	expect_filename = 0;
	while (i < len)
	{
		current_char = input[i];
		next_char = (i + 1 < len) ? input[i + 1] : '\0';
		if (current_char == '<' && next_char == '<')
		{
			handlee_heredoc(&i, &tokens);
			expect_heredoc_delim = 1;
			continue ;
		}
		if (expect_heredoc_delim)
		{
			handle_heredoc_delim(input, &i, len, &tokens);
			expect_heredoc_delim = 0;
			continue ;
		}
		if (current_char == '\'' || current_char == '"')
		{
			if (!handle_quotes(input, &i, (t_quote_info){len, current_char},
					&tokens))
			{
				return NULL;
			}
			continue ;
		}
		if (isspace(current_char))
		{
			i++;
			continue ;
		}
		if (current_char == '|')
		{
			add_token(&tokens, new_token(PIPE, "|"));
			i++;
			continue ;
		}
		if (current_char == '<' || current_char == '>')
		{
			handle_redirections(&i, (t_redirection_chars){current_char, next_char},
					&tokens, &expect_filename);
			continue ;
		}
		if (expect_filename)
		{
			handle_filename(input, &i, len, &tokens);
			expect_filename = 0;
			continue ;
		}
		if (current_char == '$')
		{
			
			handle_env_var(input, &i, len, &tokens);
			continue ;
		}
		handle_command_or_argument(input, &i, len, &tokens);
	}
	concatinate(&tokens);
	return tokens;
}
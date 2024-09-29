

#include "../minishell.h"

void	add_token(t_token **head, t_token *new_token)
{
	t_token	*current;

	if (*head == NULL)
	{
		*head = new_token;
	}
	else
	{
		current = *head;
		while (current->next)
		{
			current = current->next;
		}
		current->next = new_token;
	}
}
void	free_tokens(t_token *head) // Explicitly declare return type as void
{
	t_token *tmp;

	while (head != NULL)
	{
		tmp = head;
		head = head->next;
		free(tmp->value); // Assuming ft_strdup allocates memory
		free(tmp);
	}
}
t_token	*new_token(int type, const char *value)
{
    t_token	*token;

    token = malloc(sizeof(t_token));
    if (!token)
        return (NULL);
    token->type = type;
    token->value = ft_strdup(value);
    if (!token->value)
    {
        free(token);
        return (NULL);
    }
    token->space = 0;
    token->next = NULL;
    return (token);
}
int	get_status(void)
{
	int	status;

	waitpid(-1, &status, WNOHANG);
	return (status);
}

void	handlee_heredoc(int *i, t_token **tokens)
{
	add_token(tokens, new_token(HEREDOC, "<<"));
	*i += 2;
}

// Function to handle heredoc delimiter
void	handle_heredoc_delim(const char *input, int *i, int len,
		t_token **tokens)
{
	int		start;
	char	*delimiter;

	while (*i < len && isspace(input[*i]))
	{
		(*i)++;
	}
	start = *i;
	while (*i < len && !isspace(input[*i]))
	{
		(*i)++;
	}
	delimiter = ft_substr(input, start, *i - start);
	add_token(tokens, new_token(DELIMITER, delimiter));
	free(delimiter);
}

// Function to handle quotes
char	*get_env_value(const char *env_name)
{
	int		status;
	char	exit_status_str[12];
	char	*env_end;
	char	*name;
	char	*value;

	if (*env_name == '?')
	{
		status = get_status();
		snprintf(exit_status_str, sizeof(exit_status_str), "%d",
				WEXITSTATUS(status));
		return (ft_strdup(exit_status_str));
	}
	env_end = (char *)env_name;
	while (*env_end && (isalnum(*env_end) || *env_end == '_'))
		env_end++;
	name = ft_substr(env_name, 0, env_end - env_name);
	value = getenv(name);
	if (value)
	{
		value = ft_strdup(value);
		return (value);
	}
	ft_strdup("");
	return (value);
}
char	*remove_quotes(const char *str)
{
	int		len;
	char	*result;
	int		j;
	int		in_double_quotes;
	int		in_single_quotes;

	len = strlen(str);
	result = malloc(len + 1);
	j = 0;
	in_double_quotes = 0;
	in_single_quotes = 0;
	if (!result)
		return (NULL);
	for (int i = 0; i < len; i++)
	{
		if (str[i] == '"')
		{
			in_double_quotes = !in_double_quotes;
			// Do not add double quotes to the result
		}
		else if (str[i] == '\'')
		{
			in_single_quotes = !in_single_quotes;
			result[j++] = str[i]; // Keep single quotes
		}
		else
		{
			result[j++] = str[i];
		}
	}
	result[j] = '\0';
	// If double quotes were not properly closed, return the original string
	if (in_double_quotes)
	{
		free(result);
		return (strdup(str));
	}
	return (result);
}


char	*expand_variables(const char *str)
{
	t_expand_vars vars;

	vars.result = ft_strdup("");
	vars.temp = (char *)str;
	while ((vars.env_pos = strchr(vars.temp, '$')) && vars.env_pos[1] != '\0')
	{
		if (vars.env_pos[1] == '"')
		{
			vars.before_env = ft_substr(vars.temp, 0, vars.env_pos - vars.temp);
			vars.new_result = ft_strjoin(vars.result, vars.before_env);
			free(vars.result);
			free(vars.before_env);
			vars.result = vars.new_result;
			vars.temp = vars.env_pos + 1;
		}
		else
		{
			vars.before_env = ft_substr(vars.temp, 0, vars.env_pos - vars.temp);
			vars.env_value = get_env_value(vars.env_pos + 1);
			vars.new_result = ft_strjoin(vars.result, vars.before_env);
			free(vars.result);
			free(vars.before_env);
			vars.result = vars.new_result;
			if (vars.env_value)
			{
				vars.new_result = ft_strjoin(vars.result, vars.env_value);
				free(vars.result);
				free(vars.env_value);
				vars.result = vars.new_result;
			}
			vars.temp = vars.env_pos + 1;
			while (*vars.temp && (isalnum(*vars.temp) || *vars.temp == '_'))
				vars.temp++;
		}
	}
	vars.final_result = ft_strjoin(vars.result, vars.temp);
	free(vars.result);
	vars.unquoted_result = remove_quotes(vars.final_result);
	free(vars.final_result);
	return (vars.unquoted_result);
}
char	*remove_backslashes(const char *str)
{
	char	*result;
	char	*write_ptr;

	result = malloc(strlen(str) + 1);
	write_ptr = result;
	while (*str)
	{
		if (*str == '\\' && (*(str + 1) == '"' || *(str + 1) == '\\' || *(str
				+ 1) == '$'))
			str++;
		*write_ptr++ = *str++;
	}
	*write_ptr = '\0';
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
	if (!result)
		return (NULL);
	for (int i = 0; i < len; i++)
	{
		if (str[i] == '\'')
			in_single_quotes = !in_single_quotes;
		else
		{
			result[j++] = str[i];
		}
	}
	result[j] = '\0';
	if (in_single_quotes)
	{
		free(result);
		printf("Error: unclosed quote\n");
		
		return (strdup(str));
	}
	return (result);
}




void	handle_redirections(int *i, t_redirection_chars chars,
		t_token **tokens, int *expect_filename)
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

// Function to handle filenames
void	handle_filename(const char *input, int *i, int len, t_token **tokens)
{
	int		start;
	char	*filename;

	start = *i;
	while (*i < len && !isspace(input[*i]) && input[*i] != '|'
		&& input[*i] != '<' && input[*i] != '>' && input[*i] != '\''
		&& input[*i] != '"')
	{
		(*i)++;
	}
	filename = ft_substr(input, start, *i - start);
	add_token(tokens, new_token(FILENAME, filename));
	free(filename);
}

// Function to handle environment variables
void	handle_env_var(const char *input, int *i, int len, t_token **tokens)
{
	int		start;
	char	*env_var;
	char	**split_value;
	int		j;
	t_token	*new;

	start = *i;
	(*i)++;
	if (*i < len && input[*i] == '{')
	{
		(*i)++;
		while (*i < len && input[*i] != '}')
			(*i)++;
		if (*i < len)
			(*i)++;
	}
	else if (*i < len && (isalnum(input[*i]) || input[*i] == '_'))
	{
		while (*i < len && (isalnum(input[*i]) || input[*i] == '_'))
			(*i)++;
	}
	else
	{
		add_token(tokens, new_token(ARG, "$"));
		return ;
	}
	env_var = ft_substr(input, start, *i - start);
	char *env_value = getenv(env_var + 1); // +1 to skip the '$'
	if (env_value)
	{
		// Check if the value is quoted
		if ((env_value[0] == '"' && env_value[strlen(env_value) - 1] == '"') ||
			(env_value[0] == '\'' && env_value[strlen(env_value) - 1] == '\''))
		{
			// If quoted, add as is
			add_token(tokens, new_token(ARG, env_value));
		}
		else
		{
			// If not quoted, split and add each part as a separate token
			split_value = ft_splitD(env_value, " \t");
			printf("split_value: %s\n", split_value[0]);
			j = 0;
			while (split_value[j] != NULL)
			{
				new = new_token(ARG, split_value[j]);
				if (split_value[j + 1] != NULL)
					new->space = 1;
				add_token(tokens, new);
				free(split_value[j]);
				j++;
			}
			free(split_value);
		}
	}
	else
	{
		add_token(tokens, new_token(ARG, ""));
	}
	free(env_var);
}

// Function to handle commands and arguments
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

void	handle_command_or_argument(const char *input, int *i, int len,
		t_token **tokens)
{
	int				start;
	int				in_single_quotes;
	int				in_double_quotes;
	int				escaped;
	char			*value;
	t_token_type	type;
	t_token			*new;
	t_token			*last_token;
	char			*expanded_value;
	char			*final_value;

	start = *i;
	in_single_quotes = 0;
	in_double_quotes = 0;
	escaped = 0;
	last_token = *tokens;
	while (last_token && last_token->next)
	{
		last_token = last_token->next;
	}
	while (*i < len)
	{
		if (input[*i] == '\\' && !escaped)
		{
			escaped = 1;
		}
		else if (input[*i] == '\'' && !in_double_quotes && !escaped)
		{
			in_single_quotes = !in_single_quotes;
		}
		else if (input[*i] == '"' && !in_single_quotes && !escaped)
		{
			in_double_quotes = !in_double_quotes;
		}
		else if (!in_single_quotes && !in_double_quotes && !escaped &&
					(isspace(input[*i]) || input[*i] == '|' || input[*i] == '<'
							|| input[*i] == '>'))
		{
			break ;
		}
		escaped = 0;
		(*i)++;
	}
	// Check for unclosed quotes
	if (in_single_quotes || in_double_quotes)
	{
		fprintf(stderr, "Error: Unclosed quote\n");
		return;
	}
	value = strndup(input + start, *i - start);
	if (!value)
	{
		fprintf(stderr, "Error: Memory allocation failed\n");
		return ;
	}
	expanded_value = expand_variables(value);
	free(value);
	if (!expanded_value)
	{
		return ;
	}
	final_value = remove_single_quotes(expanded_value);
	free(expanded_value);
	if (!final_value)
	{
		return ;
	}
	if (*tokens == NULL || last_token->type == PIPE)
		type = COMMANDE;
	else
		type = ARG;
	new = new_token(type, final_value);
	

	new->space = (input[*i] == ' ');
	add_token(tokens, new);
	free_tokens(*tokens);
	free(final_value);
}


void concatinate(t_token **tokens)
{
    t_token *current = *tokens;
    t_token *next;
    char *new_value;

    while (current && current->next)
    {
        next = current->next;
        if ((current->type == ARG || current->type == COMMANDE) &&
            (next->type == ARG || next->type == COMMANDE) &&
            !current->space)
        {
            new_value = malloc(strlen(current->value) + strlen(next->value) + 1);
            if (!new_value)
            {
                fprintf(stderr, "Error: Memory allocation failed in concatinate\n");
                return;
            }
            strcpy(new_value, current->value);
            strcat(new_value, next->value);
            free(current->value);
            current->value = new_value;
            current->next = next->next;
            current->space = next->space; // Update the space flag
            free(next->value);
            free(next);
            if (!current->next) // Ensure current->next is not NULL
                break;
        }
        else
        {
            current = current->next;
        }
    }
}

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
		if (current_char == '$' && next_char == '?')
		{
			add_token(&tokens, new_token(EXIT_STATUS, "$?"));
			i += 2;
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

t_command	*new_command(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	cmd->name = NULL;
	cmd->args = malloc(sizeof(char *) * 64);
	// Start with space for 64 arguments
	cmd->arg_count = 0;
	cmd->pipe_next = 0;
	cmd->redirections = NULL;
	cmd->next = NULL;
	return cmd;
}

void	add_argument(t_command *cmd, char *arg)
{
	char	*trimmed_arg;

	trimmed_arg = ft_strtrim(arg, " \t");
	if (trimmed_arg == NULL)
	{
		// Handle memory allocation error
		fprintf(stderr, "Error: Memory allocation failed in add_argument\n");
		return ;
	}
	if (cmd->arg_count == 0)
	{
		cmd->name = ft_strdup(trimmed_arg);
	}
	cmd->args[cmd->arg_count++] = ft_strdup(trimmed_arg);
	cmd->args[cmd->arg_count] = NULL; // Ensure null-termination
	free(trimmed_arg);
}
void	add_redirection(t_command *cmd, int type, char *filename)
{
	t_redirection	*redir;

	redir = malloc(sizeof(t_redirection));
	redir->type = type;
	redir->filename = ft_strdup(filename);
	redir->next = cmd->redirections;
	cmd->redirections = redir;
}

void	add_command(t_command **list, t_command *cmd)
{
	t_command	*current;

	if (*list == NULL)
	{
		*list = cmd;
	}
	else
	{
		current = *list;
		while (current->next)
		{
			current = current->next;
		}
		current->next = cmd;
	}
}

int	validate_syntax(t_token *tokens)
{
	int		command_count;
	int		redirection_count;
	t_token	*current;

	command_count = 0;
	redirection_count = 0;
	current = tokens;
	// Ensure the first token is not a pipe
	if (current && current->type == PIPE)
	{
		fprintf(stderr, "Error: Invalid syntax near '%s'\n", current->value);
		return 0;
	}
	while (current)
	{
		if (current->type == COMMANDE || current->type == ARG)
		{
			command_count++;
			redirection_count = 0; // Reset redirection count after a command
		}
		else if (current->type == PIPE)
		{
			// Ensure a command exists before the pipe
			if (command_count == 0 && redirection_count == 0)
			{
				fprintf(stderr, "Error: Invalid syntax near '|'\n");
				return 0;
			}
			// Reset counts after a pipe
			command_count = 0;
			redirection_count = 0;
		}
		else if (current->type == OUTPUT || current->type == APPEND
				|| current->type == INPUT || current->type == HEREDOC)
		{
			// Ensure the next token exists
			if (!current->next)
			{
				fprintf(stderr,
						"Error: Missing filename or delimiter after '%s'\n",
						current->value);
				return 0;
			}
			// For heredoc, the next token can be any type (used as a delimiter)
			// For other redirections, ensure the next token is a filename
			if (current->type != HEREDOC && current->next->type != FILENAME)
			{
				fprintf(stderr, "Error: Invalid token after '%s'\n",
						current->value);
				return 0;
			}
			// Skip the filename/delimiter token
			current = current->next;
			redirection_count++;
		}
		current = current->next;
	}
	// Ensure there's at least one command or redirection in the entire input
	if (command_count == 0 && redirection_count == 0)
	{
		fprintf(stderr, "Error: No valid commands or redirections found\n");
		tokens = NULL;
		return 0 ;
	}
	return 1;
}
void	print_tokens(t_token *tokens)
{
	while (tokens)
	{
		//print type and value
		printf("Type: %d, Value: %s\n", tokens->type, tokens->value);
		tokens = tokens->next;
	}
}
t_command	*parse_tokens(t_token *tokens)
{
	t_command	*command_list;
	t_command	*current_command;
	int			status;
	char		*env_value;
	char		*heredoc_content;
	char		temp_filename[sizeof("/tmp/minishell_heredocXXXXXX")];
	int			fd;
	char		exit_status_str[12];

	command_list = NULL;
	current_command = NULL;
	status = get_status();
	if (!validate_syntax(tokens))
	{
		return NULL;
	}
	while (tokens)
	{
		if (tokens->type == COMMANDE)
		{
			if (!current_command)
			{
				current_command = new_command();
				add_command(&command_list, current_command);
			}
			add_argument(current_command, tokens->value);
		}
		else if (tokens->type == ARG)
		{
			if (!current_command)
			{
				current_command = new_command();
				add_command(&command_list, current_command);
			}
			add_argument(current_command, tokens->value);
		}
		else if (tokens->type == ENV_VAR)
		{
			if (!current_command)
			{
				current_command = new_command();
				add_command(&command_list, current_command);
			}
			env_value = getenv(tokens->value + 1);
			if (env_value)
			{
				add_argument(current_command, env_value);
			}
			else
			{
				add_argument(current_command, "");
			}
		}
		else if (tokens->type == INPUT || tokens->type == OUTPUT
				|| tokens->type == APPEND)
		{
			if (!current_command)
			{
				current_command = new_command();
				add_command(&command_list, current_command);
			}
			if (!tokens->next || (tokens->next->type != FILENAME
					&& tokens->next->type != ARG))
			{
				fprintf(stderr, "Error: Missing target after redirection\n");
				return NULL;
			}
			add_redirection(current_command, tokens->type, tokens->next->value);
			tokens = tokens->next; // Skip the filename token
		}
		else if (tokens->type == HEREDOC)
		{
			if (!current_command)
			{
				current_command = new_command();
				add_command(&command_list, current_command);
			}
			if (!tokens->next || tokens->next->type != DELIMITER)
			{
				fprintf(stderr, "Error: Missing delimiter after heredoc\n");
				return NULL;
			}
			heredoc_content = handle_heredoc(tokens->next->value, 1);
			ft_strcpy(temp_filename, "/tmp/minishell_heredocXXXXXX");
			fd = my_mkstemp(temp_filename);
			if (fd == -1)
			{
				perror("Error creating temporary file for heredoc");
				free(heredoc_content);
				return NULL;
			}
			if (heredoc_content)
			{
				write(fd, heredoc_content, strlen(heredoc_content));
				free(heredoc_content);
			}
			close(fd);
			add_redirection(current_command, HEREDOC, temp_filename);
			tokens = tokens->next; // Skip the delimiter token
		}
		else if (tokens->type == PIPE)
		{
			if (!current_command)
			{
				fprintf(stderr, "Error: Pipe without a preceding command\n");
				return NULL;
			}
			current_command->pipe_next = 1;
			current_command = NULL;
			// Check the next token after the pipe
			if (tokens->next && tokens->next->type != INPUT
				&& tokens->next->type != OUTPUT &&
				tokens->next->type != APPEND && tokens->next->type != HEREDOC)
			{
				tokens->next->type = COMMANDE; // Set the type to COMMANDE
			}
		}
		else if (tokens->type == EXIT_STATUS)
		{
			if (!current_command)
			{
				current_command = new_command();
				add_command(&command_list, current_command);
			}
			snprintf(exit_status_str, sizeof(exit_status_str), "%d",
					WEXITSTATUS(status));
			add_argument(current_command, exit_status_str);
		}
		tokens = tokens->next;
	}
	return command_list;
}
void	free_command(t_command *cmd)
{
	t_redirection	*redir;
	t_redirection	*next;

	if (cmd->name)
		free(cmd->name);
	for (int i = 0; i < cmd->arg_count; i++)
	{
		free(cmd->args[i]);
	}
	free(cmd->args);
	redir = cmd->redirections;
	while (redir)
	{
		next = redir->next;
		free(redir->filename);
		free(redir);
		redir = next;
	}
	free(cmd);
}

void	free_command_list(t_command *list)
{
	t_command	*next;

	while (list)
	{
		next = list->next;
		free_command(list);
		list = next;
	}
}

void	print_command_list(t_command *list)
{
	t_redirection	*redir;

	while (list)
	{
		printf("Command: %s\n", list->name);
		printf("Arguments:");
		for (int i = 0; i < list->arg_count; i++)
		{
			printf(" %s", list->args[i]);
		}
		printf("\n");
		redir = list->redirections;
		while (redir)
		{
			printf("Redirection: %s -> %s\n",
					redir->type == INPUT ? "INPUT" : redir->type == OUTPUT ? "OUTPUT"
																			: "APPEND",
					redir->filename);
			redir = redir->next;
		}
		if (list->pipe_next)
		{
			printf("Pipe to the next command\n");
		}
		list = list->next;
	}
}

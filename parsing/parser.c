

#include "../minishell.h"
int ft_isalpha(int c)
{
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

int ft_isalnum(int c)
{
	return (ft_isalpha(c) || ft_isdigit(c));
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
			vars.new_result = ft_strjoin(vars.result, vars.before_env);
			free(vars.result);
			free(vars.before_env);
			vars.result = vars.new_result;

			vars.temp = vars.env_pos + 1;
			while (*vars.temp && (ft_isalnum(*vars.temp) || *vars.temp == '_'))
				vars.temp++;
		}
	}
	vars.final_result = ft_strjoin(vars.result, vars.temp);
	free(vars.result);
	vars.unquoted_result = remove_quotes(vars.final_result);
	free(vars.final_result);
	return (vars.unquoted_result);
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




// Function to handle commands and arguments





void concatinate(t_token **tokens)
{
    t_token *current = *tokens;
    t_token *next;
    char *new_value;

    while (current && current->next)
    {
        next = current->next;
        if ((current->type == ARG || current->type == COMMANDE) &&
            (next->type == ARG || next->type == COMMANDE ) &&
            !current->space)
        {
            new_value = malloc(strlen(current->value) + strlen(next->value) + 1);

            if (!new_value)
                return;
            strcpy(new_value, current->value);
            strcat(new_value, next->value);
            free(current->value);
            current->value = new_value;
            current->next = next->next;
            current->space = next->space; 
            free(next->value);
            free(next);
            if (!current->next) 
                break;
        }
        else
            current = current->next;
    }
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


void	print_tokens(t_token *tokens)
{
	while (tokens)
	{
		//print type and value
		printf("Type: %d, Value: %s\n", tokens->type, tokens->value);
		tokens = tokens->next;
	}
}
int check_heredoc_delim(t_token *tokens)
{
	// Check if the token after HEREDOC is PIPE, OUTPUT, INPUT, APPEND, or another HEREDOC
	while (tokens)
	{
		if (tokens->type == HEREDOC)
		{
			if (tokens->next && (strcmp(tokens->next->value, "<") == 0 || 
			strcmp(tokens->next->value, ">") == 0 ||
			 strcmp(tokens->next->value, ">>") == 0 ||
			  strcmp(tokens->next->value, "<<") == 0 || 
			  strcmp(tokens->next->value, "|") == 0))		
			  {

				return 0;
			}
		}
		tokens = tokens->next;
	}
	return 1;
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

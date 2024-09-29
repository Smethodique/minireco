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

t_token	*new_token(int type, const char *value)
{
    t_token	*token;

    token = malloc(sizeof(t_token));
    token->type = type;
    token->value = ft_strdup(value);
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
void	handle_heredoc_delim(const char *input, int *i, int len, t_token **tokens)
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
        snprintf(exit_status_str, sizeof(exit_status_str), "%d", WEXITSTATUS(status));
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
    int		in_quotes;
    char	quote_char;

    len = strlen(str);
    result = malloc(len + 1);
    j = 0;
    in_quotes = 0;
    quote_char = 0;
    for (int i = 0; i < len; i++)
    {
        if (str[i] == '\'' || str[i] == '"')
        {
            if (!in_quotes)
            {
                in_quotes = 1;
                quote_char = str[i];
            }
            else if (str[i] == quote_char)
            {
                in_quotes = 0;
            }
        }
        else
        {
            result[j++] = str[i];
        }
    }
    result[j] = '\0';
    return (result);
}

char	*expand_variables(const char *str, t_token *tokens)
{
    char	*result;
    char	*temp;
    char	*env_pos;
    char	*before_env;
    char	*env_value;
    char	*new_result;
    char	*final_result;
    char	*unquoted_result;

    result = ft_strdup("");
    temp = (char *)str;
    while ((env_pos = strchr(temp, '$')) && env_pos[1] != '\0')
    {
        // Check if the next character is a double quote
        if (env_pos[1] == '"')
        {
            before_env = ft_substr(temp, 0, env_pos - temp);
            new_result = ft_strjoin(result, before_env);
            free(result);
            free(before_env);
            result = new_result;
            // Skip the $ and the double quote
            temp = env_pos + 1;
        }
        else
        {
            before_env = ft_substr(temp, 0, env_pos - temp);
            env_value = get_env_value(env_pos + 1);
            new_result = ft_strjoin(result, before_env);
            free(result);
            free(before_env);
            result = new_result;
            if (env_value)
            {
                new_result = ft_strjoin(result, env_value);
                free(result);
                free(env_value);
                result = new_result;
            }
            temp = env_pos + 1;
            while (*temp && (isalnum(*temp) || *temp == '_'))
                temp++;
        }
    }
    final_result = ft_strjoin(result, temp);
    free(result);
    // Preserve quotes for echo command
    if (strcmp(tokens->name, "echo") == 0)
        return final_result;
    unquoted_result = remove_quotes(final_result);
    free(final_result);
    return (unquoted_result);
}

char	*remove_backslashes(const char *str)
{
    char	*result;
    char	*write_ptr;

    result = malloc(strlen(str) + 1);
    write_ptr = result;
    while (*str)
    {
        if (*str == '\\' && (*(str + 1) == '"' || *(str + 1) == '\\' || *(str + 1) == '$'))
            str++;
        *write_ptr++ = *str++;
    }
    *write_ptr = '\0';
    return (result);
}

int	calculate_quote_num(const char *input, int len, char quote_char)
{
    int	quote_num;

    quote_num = 0;
    int i = 0;
    while (i < len)
    {
        if (input[i] == quote_char)
        {
            quote_num++;
        }
        i++;
    }
    return (quote_num);
}

int	handle_quotes(const char *input, int *i, int len, char quote_char, t_token **tokens)
{
    int start = *i;  // Changed from ++(*i) to *i
    int escaped = 0;
    char *quoted, *result, *final_result;
    t_token *last_token = NULL;
    t_token_type type;

    while (*i < len)
    {
        if (input[*i] == '\\' && !escaped)
        {
            escaped = 1;
        }
        else if (input[*i] == quote_char && !escaped)
        {
            // Allow empty single quoted strings and retain quotes
            if (*i == start)
            {
                add_token(tokens, new_token(QUOTED_STRING, "''"));
                (*i)++;
                return 1;
            }
            break;
        }
        else
        {
            escaped = 0;
        }
        (*i)++;
    }
    printf("i: %d\n", *i);
    printf("len: %d\n", len);
    if (*i >= len)
    {
        ft_putstr_fd("Error: unclosed quote\n", 2);
        return (0);
    }
    quoted = ft_substr(input, start, *i - start);
    printf("quoted: %s\n", quoted);
    if (!quoted)
        return (0);
    if (quote_char == '"')
    {
        // Expand variables only in double quotes
        result = expand_variables(quoted, *tokens);
    }
    else
    { // quote_char == '\''
        // Don't expand variables in single quotes
        result = ft_strdup(quoted);
    }
    if (!result)
        return (0);
    final_result = remove_backslashes(result);
    free(result);
    if (!final_result)
        return (0);
    printf("final_result: %s\n", final_result);
    if (*tokens == NULL || (*tokens)->type == PIPE)
        type = COMMANDE;
    else
        type = ARG;
    // Add the token without concatenation
    if (final_result[0] != '\0')
    {
        last_token = new_token(type, final_result); // Create new token
        add_token(tokens, last_token);              // Add it to the list
    }
    // Check if the next character is a space and the current token is an argument
    if (input[*i + 1] == ' ')
    {
        last_token->space = 1;
        // Set the space flag on the last token (which is "D")
        printf("space: %d for token %s \n", last_token->space, last_token->value); // This will print "D"
    }
    free(final_result);
    (*i)++;
    return (1);
}

// Function to handle redirections
void	handle_redirections(int *i, char current_char, char next_char, t_token **tokens, int *expect_filename)
{
    if (current_char == '<')
    {
        *expect_filename = 1;
        add_token(tokens, new_token(INPUT, "<"));
    }
    else if (current_char == '>' && next_char == '>')
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
    while (*i < len && !isspace(input[*i]) && input[*i] != '|' && input[*i] != '<' && input[*i] != '>' && input[*i] != '\'' && input[*i] != '"')
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
        if ((env_value[0] == '"' && env_value[strlen(env_value) - 1] == '"') || (env_value[0] == '\'' && env_value[strlen(env_value) - 1] == '\''))
        {
            // If quoted, add as is
            add_token(tokens, new_token(ARG, env_value));
        }
        else
        {
            // If not quoted, split and add each part as a separate token
            split_value = ft_splitD(env_value, " ");
            printf("split_value: %s\n", split_value[0]);
            j = 0;
            while (split_value[j] != NULL)
            {
                new = new_token(ARG, split_value[j]);
                if (split_value[j + 1] != NULL)
                {
                    add_token(tokens, new);
                    free(split_value[j]);
                    j++;
                }
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
int is_quoted(const char *str)
{
    size_t len = strlen(str);
    return (len >= 2 && ((str[0] == '"' && str[len - 1] == '"') || (str[0] == '\'' && str[len - 1] == '\'')));
}

void handle_command_or_argument(const char *input, int *i, int len, t_token **tokens)
{
    int start = *i;
    int in_single_quotes = 0;
    int in_double_quotes = 0;
    int escaped = 0;
    char *value;
    t_token_type type;
    t_token *new;

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
        else if (!in_single_quotes && !in_double_quotes && !escaped && (isspace(input[*i]) || input[*i] == '|' || input[*i] == '<' || input[*i] == '>'))
        {
            break;
        }
        escaped = 0;
        (*i)++;
    }

    if (in_single_quotes || in_double_quotes)
    {
        ft_putstr_fd("Error: unclosed quote\n", 2);
        return;
    }

    value = ft_substr(input, start, *i - start);
    if (!value)
    {
        ft_putstr_fd("Error: Memory allocation failed\n", 2);
        return;
    }

    type = (*tokens == NULL || (*tokens)->type == PIPE) ? COMMANDE : ARG;

    char *expanded_value = expand_variables(value, *tokens);
    free(value);
    if (!expanded_value)
    {
        return;
    }

    if (expanded_value[0] != '\0')
    {
        new = new_token(type, expanded_value);
        new->space = (input[*i] == ' ');
        add_token(tokens, new);
    }

    free(expanded_value);
}

void	concatinate(t_token **tokens)
{
    t_token	*current;
    t_token	*next;
    char	*concatenated;

    current = *tokens;
    while (current && current->next)
    {
        next = current->next;
        if ((current->type == ARG || current->type == COMMANDE) && (next->type == ARG || next->type == COMMANDE))
        {
            if (current->space == 0 && !is_quoted(current->value) && !is_quoted(next->value))
            {
                concatenated = ft_strjoin(current->value, next->value);
                free(current->value);
                current->value = concatenated;
                current->space = next->space;
                current->next = next->next;
                free(next->value);
                free(next);
            }
            else
            {
                current = current->next;
            }
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
    char	quote_char;
    char	current_char;
    char	next_char;

    tokens = NULL;
    i = 0;
    len = strlen(input);
    expect_heredoc_delim = 0;
    expect_filename = 0;
    quote_char = '\0';
    while (i < len)
    {
        current_char = input[i];
        next_char = (i + 1 < len) ? input[i + 1] : '\0';
        if (current_char == '<' && next_char == '<' && quote_char == '\0')
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
        if ((current_char == '\'' || current_char == '"') && quote_char == '\0')
        {
            quote_char = current_char;
            if (!handle_quotes(input, &i, len, quote_char, &tokens))
            {
                return (NULL);
            }
            // Skip the closing quote
            quote_char = '\0';
            continue ;
        }
        if (isspace(current_char) && quote_char == '\0')
        {
            i++;
            continue ;
        }
        if (current_char == '$' && next_char == '?' && quote_char == '\0')
        {
            add_token(&tokens, new_token(EXIT_STATUS, "$?"));
            i += 2;
            continue ;
        }
        if (current_char == '|' && quote_char == '\0')
        {
            add_token(&tokens, new_token(PIPE, "|"));
            i++;
            continue ;
        }
        if ((current_char == '<' || current_char == '>') && quote_char == '\0')
        {
            handle_redirections(&i, current_char, next_char, &tokens, &expect_filename);
            continue ;
        }
        if (expect_filename)
        {
            handle_filename(input, &i, len, &tokens);
            expect_filename = 0;
            continue ;
        }
        if (current_char == '$' && quote_char == '\0')
        {
            handle_env_var(input, &i, len, &tokens);
            continue ;
        }
        handle_command_or_argument(input, &i, len, &tokens);
    }
    concatinate(&tokens);
    return (tokens);
}

void	print_tokens(t_token *tokens)
{
    while (tokens)
    {
        printf("Type: %d, Value: %s\n", tokens->type, tokens->value);
        tokens = tokens->next;
    }
}

t_command	*new_command(void)
{
    t_command	*cmd;

    cmd = malloc(sizeof(t_command));
    cmd->name = NULL;
    cmd->args = malloc(sizeof(char *)
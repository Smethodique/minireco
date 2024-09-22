#include "../minishell.h"



void add_token(t_token **head, t_token *new_token) {
    if (*head == NULL) {
        *head = new_token;
    } else {
        t_token *current = *head;
        while (current->next) {
            current = current->next;
        }
        current->next = new_token;
    }
}
t_token *new_token(int type, const char *value) {
    t_token *token = malloc(sizeof(t_token));
    token->type = type;
    token->value = ft_strdup(value);
    token->space = 0;
    token->next = NULL;
    return token;
}
int get_status()
{
    int status;
    waitpid(-1, &status, WNOHANG);
    return status;
}

void handlee_heredoc( int *i, t_token **tokens) {
    add_token(tokens, new_token(HEREDOC, "<<"));
    *i += 2;
}

// Function to handle heredoc delimiter
void handle_heredoc_delim(const char *input, int *i, int len, t_token **tokens) {
    while (*i < len && isspace(input[*i])) {
        (*i)++;
    }

    int start = *i;
    while (*i < len && !isspace(input[*i])) {
        (*i)++;
    }

    char *delimiter = ft_substr(input, start, *i - start);
    add_token(tokens, new_token(DELIMITER, delimiter));

}


// Function to handle quotes
char *get_env_value(const char *env_name)
{
    if (*env_name == '?')
    {
        int status = get_status();
        char exit_status_str[12];
        snprintf(exit_status_str, sizeof(exit_status_str), "%d", WEXITSTATUS(status));
        return ft_strdup(exit_status_str);
    }

    char *env_end = (char *)env_name;
    while (*env_end && (isalnum(*env_end) || *env_end == '_'))
        env_end++;

    char *name = ft_substr(env_name, 0, env_end - env_name);
    char *value = getenv(name);
    if(value)
      {
         value = ft_strdup(value);
            return value;
       }
    ft_strdup("");
    return value;
}
char *remove_quotes(const char *str) {
    int len = strlen(str);
    char *result = malloc(len + 1);
    int j = 0;
    int in_quotes = 0;
    char quote_char = 0;

    for (int i = 0; i < len; i++) {
        if (str[i] == '\'' || str[i] == '"') {
            if (!in_quotes) {
                in_quotes = 1;
                quote_char = str[i];
            } else if (str[i] == quote_char) {
                in_quotes = 0;
            }
        } else {
            result[j++] = str[i];
        }
    }

    result[j] = '\0';
    return result;
}
char *expand_variables(const char *str) {
    char *result = ft_strdup("");
    char *temp = (char *)str;
    char *env_pos;

    while ((env_pos = strchr(temp, '$')) && env_pos[1] != '\0') {
        // Check if the next character is a double quote
        if (env_pos[1] == '"') {
            char *before_env = ft_substr(temp, 0, env_pos - temp);
            char *new_result = ft_strjoin(result, before_env);
            free(result);
            free(before_env);
            result = new_result;

            // Skip the $ and the double quote
            temp = env_pos + 1;
        } else {
            char *before_env = ft_substr(temp, 0, env_pos - temp);
            char *env_value = get_env_value(env_pos + 1);

            char *new_result = ft_strjoin(result, before_env);
            free(result);
            free(before_env);
            result = new_result;

            if (env_value) {
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

    char *final_result = ft_strjoin(result, temp);
    free(result);

    // Remove quotes from the final result
    char *unquoted_result = remove_quotes(final_result);
    free(final_result);

    return unquoted_result;
}
char *remove_backslashes(const char *str)
{
    char *result = malloc(strlen(str) + 1);
    char *write_ptr = result;

    while (*str)
    {
        if (*str == '\\' && (*(str + 1) == '"' || *(str + 1) == '\\' || *(str + 1) == '$'))
            str++;
        *write_ptr++ = *str++;
    }
    *write_ptr = '\0';

    return result;
}
int handle_quotes(const char *input, int *i, int len, char quote_char, t_token **tokens) {
    int start = ++(*i);
    char *quoted, *result, *final_result;
    t_token_type type;
    t_token *last_token = NULL; // To keep track of the last added token

    while (*i < len && input[*i] != quote_char) {
        if (quote_char == '"' && input[*i] == '\\' && *i + 1 < len &&
            (input[*i + 1] == '"' || input[*i + 1] == '\\' || input[*i + 1] == '$'))
            (*i)++;
        (*i)++;
    }
    if (*i >= len) {
        ft_putstr_fd("Error: unclosed quote\n", 2);
        return 0;
    }

    quoted = ft_substr(input, start, *i - start);
    printf("quoted: %s\n", quoted);

    if (!quoted)
        return 0;

    if (quote_char == '"') {
        // Expand variables only in double quotes
        result = expand_variables(quoted);
    } else { // quote_char == '\''
        // Don't expand variables in single quotes
        result = ft_strdup(quoted);
    }

    if (!result)
        return 0;

    final_result = remove_backslashes(result);
    free(result);

    if (!final_result)
        return 0;

    printf("final_result: %s\n", final_result);

    if (*tokens == NULL || (*tokens)->type == PIPE)
        type = COMMANDE;
    else
        type = ARG;

    // Add the token without concatenation
    if (final_result[0] != '\0') {
        last_token = new_token(type, final_result);  // Create new token
        add_token(tokens, last_token);               // Add it to the list
    }

    // Check if the next character is a space and the current token is an argument
    if (input[*i + 1] == ' ' && type == ARG && last_token) {
        last_token->space = 1;  // Set the space flag on the last token (which is "D")
        printf("space: %d for token %s \n", last_token->space, last_token->value);  // This will print "D"
    }

    free(final_result);
    (*i)++;

    return 1;
}
// Function to handle redirections
void handle_redirections(int *i, char current_char, char next_char, t_token **tokens, int *expect_filename) {
    if (current_char == '<') {
        *expect_filename = 1;
        add_token(tokens, new_token(INPUT, "<"));
    } else if (current_char == '>' && next_char == '>') {
        *expect_filename = 1;
        add_token(tokens, new_token(APPEND, ">>"));
        (*i)++;
    } else {
        *expect_filename = 1;
        add_token(tokens, new_token(OUTPUT, ">"));
    }
    (*i)++;
}


// Function to handle filenames
void handle_filename(const char *input, int *i, int len, t_token **tokens) {
    int start = *i;
    while (*i < len && !isspace(input[*i]) && input[*i] != '|' && input[*i] != '<' && input[*i] != '>' && input[*i] != '\'' && input[*i] != '"') {
        (*i)++;
    }
    char *filename = ft_substr(input, start, *i - start);
    add_token(tokens, new_token(FILENAME, filename));
    free(filename);
}

// Function to handle environment variables
void handle_env_var(const char *input, int *i, int len, t_token **tokens) {
    int start = *i;
    (*i)++;
    if (*i < len && input[*i] == '{') {
        (*i)++;
        while (*i < len && input[*i] != '}') (*i)++;
        if (*i < len) (*i)++;
    } else if (*i < len && (isalnum(input[*i]) || input[*i] == '_')) {
        while (*i < len && (isalnum(input[*i]) || input[*i] == '_')) (*i)++;
    } else {
        add_token(tokens, new_token(ARG, "$"));
        return;
    }

    char *env_var = ft_substr(input, start, *i - start);
    char *env_value = getenv(env_var + 1);  // +1 to skip the '$'

    if (env_value) {
        // Check if the value is quoted
        if ((env_value[0] == '"' && env_value[strlen(env_value) - 1] == '"') ||
            (env_value[0] == '\'' && env_value[strlen(env_value) - 1] == '\'')) {
            // If quoted, add as is
            add_token(tokens, new_token(ARG, env_value));
        } else {
            // If not quoted, split and add each part as a separate token
            char **split_value = ft_splitD(env_value, " ");
            printf("split_value: %s\n", split_value[0]);
            int j = 0;
            while (split_value[j] != NULL) {
                t_token *new = new_token(ARG, split_value[j]);
                if (split_value[j + 1] != NULL) {
                    new->space = 1;  // Set the space flag if there are more tokens
                }
                add_token(tokens, new);
                free(split_value[j]);
                j++;
            }
            free(split_value);
        }
    } else {
        add_token(tokens, new_token(ARG, ""));
    }

    free(env_var);
}

// Function to handle commands and arguments

void handle_command_or_argument(const char *input, int *i, int len, t_token **tokens) {
    int start = *i;
    int in_quotes = 0;
    char quote_char = 0;
    int space = 0;
    t_token *last_token = *tokens;

    // Find the last token
    while (last_token && last_token->next) {
        last_token = last_token->next;
    }

    while (*i < len) {
        if (input[*i] == '\'' || input[*i] == '"') {
            if (!in_quotes) {
                in_quotes = 1;
                quote_char = input[*i];
            } else if (input[*i] == quote_char) {
                in_quotes = 0;
            }
        } else if (!in_quotes && (isspace(input[*i]) || input[*i] == '|' || input[*i] == '<' || input[*i] == '>')) {
            if (input[*i] == ' ') {
                space = 1;
            }
            break;
        }
        (*i)++;
    }

    char *value = ft_substr(input, start, *i - start);

    if (!value) {
        ft_putstr_fd("Error: Memory allocation failed\n", 2);
        return;
    }

    t_token_type type = ARG;
    if (last_token == NULL || last_token->type == PIPE) {
        type = COMMANDE;
    }

    char *expanded_value = expand_variables(value);
    free(value);

    if (!expanded_value) {
        return;
    }

    if (expanded_value[0] != '\0') {
        t_token *new = new_token(type, expanded_value);
        new->space = space;
        add_token(tokens, new);
    }

    free(expanded_value);
}
void concatinate(t_token **tokens) {
    t_token *current = *tokens;

    while (current) {
        if (current->type == COMMANDE || current->type == ARG) {
            t_token *next = current->next;
            while (next && (next->type == ARG || next->type == COMMANDE)) {
                if (current->space == 1 || next->space == 1) {
                    // If there's a space, stop concatenation.
                    break;
                } else {
                    // Concatenate arguments if there's no space and not within quotes.
                    char *new_value = ft_strjoin(current->value, next->value);
                    free(current->value);
                    current->value = new_value;

                    // Remove the next token from the list
                    t_token *temp = next;
                    current->next = next->next;
                    free(temp->value);
                    free(temp);
                    next = current->next;
                }
            }
        }
        current = current->next;
    }
}



t_token *tokenize_input(const char *input) {
    t_token *tokens = NULL;
    int i = 0;
    int len = strlen(input);

    int expect_heredoc_delim = 0;
    int expect_filename = 0;
    char quote_char = '\0';

    while (i < len) {
        char current_char = input[i];
        char next_char = (i + 1 < len) ? input[i + 1] : '\0';

        if (current_char == '<' && next_char == '<' && quote_char == '\0') {
            handlee_heredoc(&i, &tokens);
            expect_heredoc_delim = 1;
            continue;
        }

        if (expect_heredoc_delim) {
            handle_heredoc_delim(input, &i, len, &tokens);
            expect_heredoc_delim = 0;
            continue;
        }

        if ((current_char == '\'' || current_char == '"') && quote_char == '\0') {
            quote_char = current_char;
            if (!handle_quotes(input, &i, len, quote_char, &tokens)) {
                return NULL;
            }
            // Skip the closing quote


            quote_char = '\0';
            continue;
        }

        if (isspace(current_char) && quote_char == '\0') {
            i++;
            continue;
        }

        if (current_char == '$' && next_char == '?' && quote_char == '\0') {
            add_token(&tokens, new_token(EXIT_STATUS, "$?"));
            i += 2;
            continue;
        }

        if (current_char == '|' && quote_char == '\0') {
            add_token(&tokens, new_token(PIPE, "|"));
            i++;
            continue;
        }

        if ((current_char == '<' || current_char == '>') && quote_char == '\0') {
            handle_redirections(&i, current_char, next_char, &tokens, &expect_filename);
            continue;
        }

        if (expect_filename) {
            handle_filename(input, &i, len, &tokens);
            expect_filename = 0;
            continue;
        }

        if (current_char == '$' && quote_char == '\0') {
            handle_env_var(input, &i, len, &tokens);
            continue;
        }

        handle_command_or_argument(input, &i, len, &tokens);
    }
      concatinate(&tokens);

    return tokens;
}
void print_tokens(t_token *tokens) {
    while (tokens) {
        printf("Type: %d, Value: %s\n", tokens->type, tokens->value);
        tokens = tokens->next;
    }
}

t_command *new_command() {
    t_command *cmd = malloc(sizeof(t_command));
    cmd->name = NULL;
    cmd->args = malloc(sizeof(char*) * 64);  // Start with space for 64 arguments
    cmd->arg_count = 0;
    cmd->pipe_next = 0;
    cmd->redirections = NULL;
    cmd->next = NULL;
    return cmd;
}

void add_argument(t_command *cmd, char *arg) {
    char *trimmed_arg = ft_strtrim(arg, " \t");
    if (trimmed_arg == NULL) {
        // Handle memory allocation error
        fprintf(stderr, "Error: Memory allocation failed in add_argument\n");
        return;
    }

    if (cmd->arg_count == 0) {
        cmd->name = ft_strdup(trimmed_arg);
    }
    cmd->args[cmd->arg_count++] = ft_strdup(trimmed_arg);
    cmd->args[cmd->arg_count] = NULL;  // Ensure null-termination

    free(trimmed_arg);
}
void add_redirection(t_command *cmd, int type, char *filename) {
    t_redirection *redir = malloc(sizeof(t_redirection));
    redir->type = type;
    redir->filename = ft_strdup(filename);
    redir->next = cmd->redirections;
    cmd->redirections = redir;
}

void add_command(t_command **list, t_command *cmd) {
    if (*list == NULL) {
        *list = cmd;
    } else {
        t_command *current = *list;
        while (current->next) {
            current = current->next;
        }
        current->next = cmd;
    }
}

int validate_syntax(t_token *tokens) {
    int command_count = 0;
    int redirection_count = 0;
    t_token *current = tokens;

    // Ensure the first token is not a pipe
    if (current && current->type == PIPE) {
        fprintf(stderr, "Error: Invalid syntax near '%s'\n", current->value);
        return 0;
    }

    while (current) {
        if (current->type == COMMANDE || current->type == ARG) {
            command_count++;
            redirection_count = 0;  // Reset redirection count after a command
        } else if (current->type == PIPE) {
            // Ensure a command exists before the pipe
            if (command_count == 0 && redirection_count == 0) {
                fprintf(stderr, "Error: Invalid syntax near '|'\n");
                return 0;
            }
            // Reset counts after a pipe
            command_count = 0;
            redirection_count = 0;
        } else if (current->type == OUTPUT || current->type == APPEND || current->type == INPUT || current->type == HEREDOC) {
            // Ensure the next token exists
            if (!current->next) {
                fprintf(stderr, "Error: Missing filename or delimiter after '%s'\n", current->value);
                return 0;
            }
            // For heredoc, the next token can be any type (used as a delimiter)
            // For other redirections, ensure the next token is a filename
            if (current->type != HEREDOC && current->next->type != FILENAME) {
                fprintf(stderr, "Error: Invalid token after '%s'\n", current->value);
                return 0;
            }
            // Skip the filename/delimiter token
            current = current->next;
            redirection_count++;
        }
        current = current->next;
    }

    // Ensure there's at least one command or redirection in the entire input
    if (command_count == 0 && redirection_count == 0) {
        fprintf(stderr, "Error: No valid commands or redirections found\n");
        return 0;
    }

    return 1;
}
t_command *parse_tokens(t_token *tokens) {
    t_command *command_list = NULL;
    t_command *current_command = NULL;
    int expect_heredoc_delim = 0;
    int status = get_status();

    if (!validate_syntax(tokens)) {
        return NULL;
    }

    while (tokens) {
        if (tokens->type == COMMANDE) {
            if (!current_command) {
                current_command = new_command();
                add_command(&command_list, current_command);
            }
            add_argument(current_command, tokens->value);
        } else if (tokens->type == ARG) {
            if (!current_command) {
                current_command = new_command();
                add_command(&command_list, current_command);
            }
            add_argument(current_command, tokens->value);
        } else if (tokens->type == ENV_VAR) {
            if (!current_command) {
                current_command = new_command();
                add_command(&command_list, current_command);
            }
            char *env_value = getenv(tokens->value + 1);
            if (env_value) {
                add_argument(current_command, env_value);
            } else {
                add_argument(current_command, "");
            }
        } else if (tokens->type == INPUT || tokens->type == OUTPUT || tokens->type == APPEND) {
            if (!current_command) {
                current_command = new_command();
                add_command(&command_list, current_command);
            }
            if (!tokens->next || (tokens->next->type != FILENAME && tokens->next->type != ARG)) {
                fprintf(stderr, "Error: Missing target after redirection\n");
                return NULL;
            }
            add_redirection(current_command, tokens->type, tokens->next->value);
            tokens = tokens->next; // Skip the filename token
        } else if (tokens->type == HEREDOC) {
            if (!current_command) {
                current_command = new_command();
                add_command(&command_list, current_command);
            }
            expect_heredoc_delim = 1;
        } else if (tokens->type == DELIMITER) {
            if (!expect_heredoc_delim) {
                fprintf(stderr, "Error: Unexpected delimiter '%s'\n", tokens->value);
                return NULL;
            }
            char *heredoc_content = handle_heredoc(tokens->value, 1);
            char temp_filename[] = "/tmp/minishell_heredocXXXXXX";
            int fd = my_mkstemp(temp_filename);
            if (fd == -1) {
                perror("Error creating temporary file for heredoc");
                free(heredoc_content);
                return NULL;
            }
            if (!heredoc_content) {
                return NULL;
            }
            write(fd, heredoc_content, strlen(heredoc_content));
            close(fd);
            free(heredoc_content);
            add_redirection(current_command, HEREDOC, temp_filename);
            expect_heredoc_delim = 0;
        } else if (tokens->type == PIPE) {
            if (!current_command) {
                fprintf(stderr, "Error: Pipe without a preceding command\n");
                return NULL;
            }
            current_command->pipe_next = 1;
            current_command = NULL; // Reset current_command to detect new command after pipe
        } else if (tokens->type == EXIT_STATUS) {
            if (!current_command) {
                current_command = new_command();
                add_command(&command_list, current_command);
            }
            char exit_status_str[12];
            snprintf(exit_status_str, sizeof(exit_status_str), "%d", WEXITSTATUS(status));
            add_argument(current_command, exit_status_str);
        } else {
            fprintf(stderr, "Error: Unexpected token type\n");
            return NULL;
        }

        tokens = tokens->next;
    }

    if (expect_heredoc_delim) {
        fprintf(stderr, "Error: Missing heredoc delimiter\n");
        return NULL;
    }

    return command_list;
}
void free_command(t_command *cmd) {
    if (cmd->name) free(cmd->name);
    for (int i = 0; i < cmd->arg_count; i++) {
        free(cmd->args[i]);
    }
    free(cmd->args);
    
    t_redirection *redir = cmd->redirections;
    while (redir) {
        t_redirection *next = redir->next;
        free(redir->filename);
        free(redir);
        redir = next;
    }
    
    free(cmd);
}

void free_command_list(t_command *list) {
    while (list) {
        t_command *next = list->next;
        free_command(list);
        list = next;
    }
}

void print_command_list(t_command *list) {
    while (list) {
        printf("Command: %s\n", list->name);
        printf("Arguments:");
      //split the arguments and print them use split
        for (int i = 0; i < list->arg_count; i++) {
            printf(" %s", list->args[i]);
        }

        printf("\n");

        t_redirection *redir = list->redirections;
        while (redir) {
            printf("Redirection: %s -> %s\n", redir->type == INPUT ? "INPUT" : redir->type == OUTPUT ? "OUTPUT" : "APPEND", redir->filename);
            redir = redir->next;
        }

        list = list->next;
    }
}
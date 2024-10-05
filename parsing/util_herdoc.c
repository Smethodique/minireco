#include "../minishell.h"
//include bool library
#include <stdbool.h>

int myrand(void)
{
    static unsigned int seed = 0;
    seed = seed * 1103515245 + 12345;
    return ((unsigned int)(seed / 65536) % 32768);
}



int create_temp_file(char *template) {
    int fd;
    while (1) {
        int i = 0;
        while (i < 6) {
            
            template[strlen(template) - 6 + i] = 'a' + myrand() % 26;
            i++;
        }
        fd = open(template, O_RDWR | O_CREAT | O_EXCL, 0600);
        if (fd >= 0)
            break;
    }
    return fd;
}
int my_mkstemp(char *template) {
    int fd;
    char *p ;
    int len ;

    len = strlen(template);
    p = NULL;
    if (len < 6)
        return -1;
    p = template + len - 6;
    int i = 0;
    while (i < 6) {
        if (p[i] != 'X') {
            return -1;
        }
        i++;
    }
    fd = create_temp_file(template);
    if (fd < 0) {
        perror("mkstemp");
        return -1;
    }
    return fd;
}

char *init_result_buffer(const char *input) {
    if (!input) return NULL;
    size_t result_cap = strlen(input) * 2;  // Initial capacity
    char *result = malloc(result_cap);
    return result;
}

// Extracts environment variable name from the input string
char *extract_var_name(const char *var_start, size_t *var_name_len) {
    const char *var_end = var_start;
    while (ft_isalnum(*var_end) || *var_end == '_') var_end++;
    *var_name_len = var_end - var_start;
    
    char *var_name = malloc(*var_name_len + 1);
    if (!var_name) return NULL;
    
    ft_strncpy(var_name, var_start, *var_name_len);
    var_name[*var_name_len] = '\0';
    
    return var_name;
}

// Ensures the result buffer has enough capacity
char *ensure_capacity(char *result, size_t needed_size, size_t *result_cap) {
    if (needed_size >= *result_cap) {
        *result_cap = needed_size * 2;
        char *new_result = malloc(*result_cap);
        if (!new_result) {
            free(result);
            return NULL;
        }
        strcpy(new_result, result);
        free(result);
        return new_result;
    }
    return result;
}

// Main function that expands environment variables
char *expand_env_vars(char *input) {
    if (!input) return NULL;

    size_t result_len = 0;
    size_t result_cap = strlen(input) * 2;
    char *result = init_result_buffer(input);
    if (!result) return NULL;

    char *p = input;
    while (*p) {
        if (*p == '$' && (ft_isalnum(*(p+1)) || *(p+1) == '_')) {
            size_t var_name_len;
            char *var_name = extract_var_name(p + 1, &var_name_len);
            if (!var_name) {
                free(result);
                return NULL;
            }

            const char *var_value = getenv(var_name);
            free(var_name);

            if (var_value) {
                size_t value_len = ft_strlen(var_value);
                result = ensure_capacity(result, result_len + value_len, &result_cap);
                if (!result) return NULL;
                
                strcpy(result + result_len, var_value);
                result_len += value_len;
            }

            p += var_name_len + 1;  // Skip the variable name and $ symbol
        } else {
            result = ensure_capacity(result, result_len + 1, &result_cap);
            if (!result) return NULL;
            
            result[result_len++] = *p++;
        }
    }

    result[result_len] = '\0';
    return result;
}
char *init_heredoc_buffer(void) {
    return NULL;  // Initially empty
}

// Process delimiter and check if it's quoted
char *process_delimiter(const char *delimiter, bool *is_quoted) {
    if (!delimiter) {
        ft_putstr_fd("Error: NULL delimiter\n", 2);
        return NULL;
    }

    char *unquoted_delimiter = remove_quotes((char *)delimiter);
    *is_quoted = (strcmp(delimiter, unquoted_delimiter) != 0);
    return unquoted_delimiter;
}

// Ensure buffer has enough capacity for new content
char *ensure_heredoc_capacity(char *content, size_t current_size, size_t needed_size, 
                            size_t *content_capacity) {
  (void)current_size;                              
    if (needed_size > *content_capacity) {
        *content_capacity = needed_size * 2;
        char *new_content = realloc(content, *content_capacity);
        if (!new_content) {
            free(content);
            return NULL;
        }
        return new_content;
    }
    return content;
}

// Process a single line of heredoc input
char *process_heredoc_line(char *line, bool expand_vars, bool is_quoted) {
    if (!expand_vars || is_quoted) {
        return line;
    }

    char *processed_line = expand_env_vars(line);
    if (!processed_line) {
        free(line);
        return NULL;
    }
    return processed_line;
}

// Main heredoc handling function
char *read_heredoc_content(const char *unquoted_delimiter, int expand_vars, bool is_quoted) {
    char *content = init_heredoc_buffer();
    size_t content_size = 0;
    size_t content_capacity = 0;
      
  
    while (1) {
        char *line = readline("> ");
        if (!line || ft_strcmp(line, unquoted_delimiter) == 0) {
            free(line);
            break;
        }

        char *processed_line = process_heredoc_line(line, expand_vars, is_quoted);
        if (!processed_line) {
            free(content);
            return NULL;
        }

        size_t line_len = strlen(processed_line);
        size_t new_size = content_size + line_len + 2;

        content = ensure_heredoc_capacity(content, content_size, new_size, &content_capacity);
        if (!content) {
            if (processed_line != line) free(processed_line);
            free(line);
            return NULL;
        }

        ft_memcpy(content + content_size, processed_line, line_len);
        content[content_size + line_len] = '\n';
        content_size += line_len + 1;

        if (processed_line != line) {
            free(processed_line);
        }
        free(line);
    }

    if (content) {
        content[content_size] = '\0';
    }
    return content;
}

char *handle_heredoc(const char *delimiter, int expand_vars) {
    bool is_quoted;
    char *unquoted_delimiter = process_delimiter(delimiter, &is_quoted);
    if (!unquoted_delimiter) {
        return NULL;
    }

    int pipe_fd[2];
    if (pipe(pipe_fd) == -1) {
        free(unquoted_delimiter);
        return NULL;
    }

    pid_t pid = fork();
    if (pid == -1) {
       
        free(unquoted_delimiter);
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        return NULL;
    }

    char *content = NULL;

    if (pid == 0) {  // Child process
        close(pipe_fd[0]);  // Close read end
        content = read_heredoc_content(unquoted_delimiter, expand_vars, is_quoted);
        
        if (content) {
            write(pipe_fd[1], content, strlen(content));
            free(content);
        }
        close(pipe_fd[1]);
        free(unquoted_delimiter);
        exit(0);  // Exit child process
    } else {  // Parent process
        close(pipe_fd[1]);  // Close write end
        
        // Read content from pipe
        char buffer[4096];
        ssize_t bytes_read;
        size_t total_size = 0;
        size_t capacity = 4096;
        content = malloc(capacity);

        while ((bytes_read = read(pipe_fd[0], buffer, sizeof(buffer))) > 0) {
            if (total_size + bytes_read >= capacity) {
                capacity *= 2;
                char *new_content = realloc(content, capacity);
                if (!new_content) {
                    free(content);
                    close(pipe_fd[0]);
                    free(unquoted_delimiter);
                    return NULL;
                }
                content = new_content;
            }
            memcpy(content + total_size, buffer, bytes_read);
            total_size += bytes_read;
        }

        if (content) {
            content[total_size] = '\0';
        }

        close(pipe_fd[0]);
        int status;
        waitpid(pid, &status, 0);

        // Check if child was terminated by signal
        if (WIFSIGNALED(status )) {
            free(content);
         
            content = NULL;
            return NULL;
        }
    }

    free(unquoted_delimiter);
    return content;
}

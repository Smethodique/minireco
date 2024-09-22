#include "../minishell.h"

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

char *expand_env_vars(char *input) {
    if (!input) return NULL;

    size_t result_len = 0;
    size_t result_cap = strlen(input) * 2;  // Initial capacity
    char *result = malloc(result_cap);
    if (!result) return NULL;

     char *p = input;
    while (*p) {
        if (*p == '$' && (isalnum(*(p+1)) || *(p+1) == '_')) {
            // Found a potential environment variable
             char *var_start = p + 1;
             char *var_end = var_start;
            while (isalnum(*var_end) || *var_end == '_') var_end++;

            size_t var_name_len = var_end - var_start;
            char *var_name = malloc(var_name_len + 1);
            if (!var_name) {
                free(result);
                return NULL;
            }
            ft_strncpy(var_name, var_start, var_name_len);
            var_name[var_name_len] = '\0';

            const char *var_value = getenv(var_name);
            free(var_name);

            if (var_value) {
                size_t value_len = ft_strlen(var_value);
                if (result_len + value_len >= result_cap) {
                    result_cap = (result_len + value_len) * 2;
                    char *new_result = realloc(result, result_cap);
                    if (!new_result) {
                        free(result);
                        return NULL;
                    }
                    result = new_result;
                }
                strcpy(result + result_len, var_value);
                result_len += value_len;
            }

            p = var_end;
        } else {
            // Regular character
            if (result_len + 1 >= result_cap) {
                result_cap *= 2;
                //use malloc to allocate new memory not realloc
                char *new_result = malloc(result_cap);

                if (!new_result) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }
            result[result_len++] = *p++;
        }
    }

    result[result_len] = '\0';
    return result;
}

char *handle_heredoc(const char *delimiter, int expand_vars)
{
    if (!delimiter) {
        fprintf(stderr, "Error: Null delimiter\n");
        return NULL;
    }

    char *unquoted_delimiter = remove_quotes((char *)delimiter);
    int is_quoted = (strcmp(delimiter, unquoted_delimiter) != 0);

    char *line;
    char *content = NULL;
    size_t content_size = 0;
    size_t content_capacity = 0;

    while (1) {
        line = readline("> ");
        if (!line) {
            break;
        }

        if (ft_strcmp(line, unquoted_delimiter) == 0) {
            free(line);
            break;
        }

        char *processed_line = line;
        if (expand_vars && !is_quoted) {
            processed_line = expand_env_vars(line);
            if (!processed_line) {
                free(line);
                free(content);
                free(unquoted_delimiter);
                return NULL; // Memory allocation failed
            }
        }

        size_t line_len = strlen(processed_line);
        size_t new_size = content_size + line_len + 2; // +2 for newline and null terminator

        if (new_size > content_capacity) {
            content_capacity = new_size * 2; // Double the capacity
            char *new_content = realloc(content, content_capacity);
            if (!new_content) {
                free(line);
                if (expand_vars && processed_line != line) free(processed_line);
                free(content);
                free(unquoted_delimiter);
                return NULL; // Memory allocation failed
            }
            content = new_content;
        }

        ft_memcpy(content + content_size, processed_line, line_len);
        content[content_size + line_len] = '\n';
        content_size += line_len + 1;

        if (expand_vars && processed_line != line)
            free(processed_line);
        free(line);
    }

    if (content) {
        content[content_size] = '\0';
    }

    free(unquoted_delimiter);
    return content;
}
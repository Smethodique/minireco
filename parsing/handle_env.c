#include "../minishell.h"


void handle_env_var_value(char *env_var, t_token **tokens)
{
    t_env_var_data data;

    if (env_var[1] == '?')
    {
        char *exit_status_str = ft_itoa(g_vars.exit_status);
        t_token *new_tok = new_token(ARG, exit_status_str);
        add_token(tokens, new_tok);
        free(exit_status_str);
        return;
    }
    else
    {
        data.env_value = get_env_value(env_var + 1,g_vars.env); // +1 to skip the '$'
        if (data.env_value)
        {
            if (is_quoted(data.env_value))
                add_token(tokens, new_token(ARG, data.env_value));
            else
            {
                data.split_value = ft_splitD(data.env_value, " \t");
                data.j = 0;
                while (data.split_value[data.j] != NULL)
                {
                    data.new = new_token(ARG, data.split_value[data.j]);
                    if (data.split_value[data.j + 1] != NULL)
                        data.new->space = 1;
                    add_token(tokens, data.new);
                    free(data.split_value[data.j]);
                    data.j++;
                }
                free(data.split_value);
            }
        }
        else
        {
            add_token(tokens, new_token(ARG, ""));
        }
    }
}

void handle_env_var(const char *input, int *i, int len, t_token **tokens)
{
    int start;
    char *env_var;
    int space_after_env_var;

    start = (*i)++;
    if (*i < len && input[*i] == '?')
    {
        (*i)++;
    }
    else if (*i < len && (ft_isalnum(input[*i]) || input[*i] == '_'))
    {
        while (*i < len && (ft_isalnum(input[*i]) || input[*i] == '_'))
        {
            (*i)++;
        }
    }
    else
    {
        add_token(tokens, new_token(ARG, "$"));
        return;
    }

    env_var = ft_substr(input, start, *i - start);
    space_after_env_var = (*i < len && (input[*i] == ' ' || input[*i] == '\t'));

    handle_env_var_value(env_var, tokens);

    // Set the space attribute for the last token if there is space after
    if (space_after_env_var)
    {
        t_token *last_token = *tokens;
        while (last_token->next != NULL)
        {
            last_token = last_token->next;
        }
        last_token->space = 1;
    }

    free(env_var);
}

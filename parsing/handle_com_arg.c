#include "../minishell.h"

typedef struct s_handle_vars
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
}				t_handle_vars;

void initialize_handle_vars(t_handle_vars *vars, const char *input, int *i, t_token **tokens)
{
    (void)input;
    vars->start = *i;
    vars->in_single_quotes = 0;
    vars->in_double_quotes = 0;
    vars->escaped = 0;
    vars->last_token = *tokens;
    while (vars->last_token && vars->last_token->next)
        vars->last_token = vars->last_token->next;
}



void parse_comm(const char *input, int *i, int len, t_handle_vars *vars)
{
    while (*i < len)
    {
        
         if (input[*i] == '\'' && !vars->in_double_quotes && !vars->escaped)
            vars->in_single_quotes = !vars->in_single_quotes;
        else if (input[*i] == '"' && !vars->in_single_quotes && !vars->escaped)
            vars->in_double_quotes = !vars->in_double_quotes;
        else if (!vars->in_single_quotes && !vars->in_double_quotes && !vars->escaped &&
                    (isspace(input[*i]) || input[*i] == '|' || input[*i] == '<'
                            || input[*i] == '>'))
            break ;
        vars->escaped = 0;
        (*i)++;
    }
}

void process_value(const char *input, int *i, t_handle_vars *vars, t_token **tokens)
{
    vars->value = strndup(input + vars->start, *i - vars->start);
    if (!vars->value)
    {
        ft_putstr_fd("Error: failed to allocate memory\n", 2);
        return ;
    }
    vars->expanded_value = expand_variables(vars->value);
    free(vars->value);
    if (!vars->expanded_value)
        return ;
    vars->final_value = ft_strdup(vars->expanded_value);
    free(vars->expanded_value);
    if (!vars->final_value)
        return ;
    if (*tokens == NULL || vars->last_token->type == PIPE)
        vars->type = COMMANDE;
    else
        vars->type = ARG;
    vars->new = new_token(vars->type, vars->final_value);
    vars->new->space = (input[*i] == ' ');
    add_token(tokens, vars->new);
    free(vars->final_value);
}
void handle_command_or_argument(const char *input, int *i, int len, t_token **tokens)
{
    t_handle_vars vars;
  

    initialize_handle_vars(&vars, input, i, tokens);
    parse_comm(input, i, len, &vars);
    if (vars.in_single_quotes || vars.in_double_quotes)
    {
        ft_putstr_fd("Error: unclosed quote\n", 2);
        g_exit_status = 2;
        free_tokens(*tokens);
        *tokens = NULL;
        return ;
    }
    process_value(input, i, &vars, tokens);
}

#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <errno.h>
#include "1337Libft/libft.h"
#define NOT_BUILT_IN -1

typedef struct s_env {
    char *key;
    char *value;
    struct s_env *next;
} t_env;

typedef struct s_global_vars {
    int exit_status;
    char **env;
} t_global_vars;

extern t_global_vars g_vars;

typedef enum {
   COMMANDE,
   ARG,
    REDIRECTION,
    PIPE,
    APPEND,
    EMPTY_STRING,
    INPUT,
    OUTPUT,
    HEREDOC,
    FILENAME,
    DELIMITER,
    ENV_VAR,
    SINGLE_QUOTE,
    DOUBLE_QUOTE,
    EXIT,

} t_token_type;

typedef struct s_token {
    int type;
    int quoted;
    char *value;
    int space;
    int is_env_var;
    struct s_token *next;
    
} t_token;

typedef struct s_command {
    char *name;
    char **args;
    int arg_count;
    int pipe_next;
    struct s_redirection *redirections;
    struct s_command *next;
} t_command;

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
typedef struct s_quote_state
{
	int	i;
	int	j;
	int	in_double_quotes;
	int	in_single_quotes;
}	t_quote_state;

typedef struct s_lexer_state
{
	t_token	*tokens;
	int		i;
	int		len;
	int		expect_heredoc_delim;
	int		expect_filename;
	char	current_char;
	char	next_char;
}	t_lexer_state;

typedef struct s_expansion
{
	char	*result;
	char	*temp;
	char	*new_result;
	char	*before_env;
	char	*env_pos;
	char	*env_name;
	char	*env_value;
	int		env_len;
	int		in_single_quote;
	int		in_double_quote;
	char	*final_result;
	char	*unquoted_result;
}	t_expansion;

typedef struct s_redirection {
    int type;  // INPUT, OUTPUT, or APPEND
    char *filename;
    struct s_redirection *next;
} t_redirection;

typedef struct s_tokenizer_params {
    const char *input;
    int i;
    int len;
    int expect_command;
    int expect_filename;
    t_token **tokens;
} t_tokenizer_params;

typedef struct s_heredoc_manager {
    pid_t *heredoc_pids;  // Array to track child PIDs
    int heredoc_count;    // Number of active heredocs
} t_heredoc_manager;

typedef struct s_redirection_chars
{
	char current_char;
	char next_char;
} t_redirection_chars;

typedef struct s_quote_info
{
	int len;
	char quote_char;
} t_quote_info;

typedef struct s_handle_quotes_params
{
    const char *input;
    int *i;
    t_quote_info quote_info;
    t_token **tokens;
} t_handle_quotes_params;

typedef struct s_expand_vars
{
	char	*result;
	char	*temp;
	char	*env_pos;
	char	*before_env;
	char	*env_value;
	char	*new_result;
	char	*final_result;
	char	*unquoted_result;
}	t_expand_vars;

typedef struct s_quote_result
{
    char *quoted;
    char *result;
    char *final_result;
} t_quote_result;

typedef struct s_quote_vars
{
	int		len;
	char	*result;
	int		j;
	int		in_double_quotes;
	int		in_single_quotes;
	int		i;
}	t_quote_vars;

typedef struct s_parse_context
{
    t_command *command_list;
    t_command *current_command;
    int status;
    char *env_value;
    char *heredoc_content;
    char temp_filename[sizeof("/tmp/minishell_heredocXXXXXX")];
    int fd;
    char exit_status_str[12];
} t_parse_context;

typedef struct s_env_var_data
{
	char	*env_value;
	char	**split_value;
	int		j;
	t_token	*new;
}	t_env_var_data;
// Existing function prototypes
void init_shell();
void sigint_handler(int sig);
void sigquit_handler(int sig);
void all_signals();
int is_quoted(const char *str);


t_token *new_token(int type, const char *value);
void add_token(t_token **head, t_token *new_token);
t_token *tokenize_input(const char *input);
int handle_quotes(const char *input, int *i, t_quote_info quote_info, t_token **tokens);
char *remove_quotes(const char *str);

void print_tokens(t_token *tokens);
int	calculate_quote_num(const char *input, int len, int *j, int *p);
int is_valid_delimiter(const char *delimiter);
int my_mkstemp(char *template);
char	*expand_variables(const char *str);




char *handle_heredoc(const char *delimiter, int expand_vars);


void handle_command_or_argument(const char *input, int *i, int len, t_token **tokens);  
void free_tokens(t_token *head);
void update_current_and_next_char(t_lexer_state *state, const char *input);
void initialize_state(t_lexer_state *state, const char *input);
int handle_heredoc_cases(t_lexer_state *state, const char *input);
int handle_whitespace(t_lexer_state *state);
int handle_pipe(t_lexer_state *state);
t_command *new_command();
void add_argument(t_command *cmd, char *arg);
void add_redirection(t_command *cmd, int type, char *filename);
char	*expand_variables(const char *str);
void add_command(t_command **list, t_command *cmd);
t_command *parse_tokens(t_token *tokens);
void free_command(t_command *cmd);
int	get_status(void);
int validate_syntax(t_token *tokens);
char	*remove_quotes(const char *str) ;
char	*remove_single_quotes(const char *str);
void	handlee_heredoc(int *i, t_token **tokens);
void	handle_heredoc_delim(const char *input, int *i, int len, t_token **tokens);
void	handle_redirections(int *i, t_redirection_chars chars, t_token **tokens, int *expect_filename);
void	handle_filename(const char *input, int *i, int len, t_token **tokens);
void	handle_env_var(const char *input, int *i, int len, t_token **tokens);
void    concatinate(t_token **tokens);
int check_heredoc_delim(t_token *tokens);
void free_command_list(t_command *list);
void print_command_list(t_command *list);
char *remove_enclosing_quotes( char *str);
 void parse_token_one(t_parse_context *ctx, t_token **tokens);
 void parse_token_two(t_parse_context *ctx, t_token **tokens);
 void parse_token_three(t_parse_context *ctx, t_token **tokens);
 void parse_token_four(t_parse_context *ctx, t_token **tokens);
 void parse_token_five(t_parse_context *ctx, t_token **tokens);
 pid_t execute_piped_command(t_command *cmd, int in_fd, int out_fd, char **env);

 void	update_old_pwd(char *old_pwd, char **env);
void	update_pwd(char *pwd, char **env);
char	*ft_chr(char **env, char *vrb);
void    cd(t_command *cmd, char **env);
int echo(t_command *cmd, char **env);
int first_non_option(char **args);
int is_n_option(char *arg);
void env(t_command *cmd, char **env);
void handle_pipes(t_command *commands, char **env);
int	double_pointer_len(char **str);
void	ft_setter(int value);
int	ft_getter(void);
int is_builtin(t_command *cmd);
int	is_num(char *str);
void export(t_command *cmd, char **env);
int check_export(char *cmd);
void    export_helper(char *cmd, char **env, int len);
int check_env(char *cmd, char **env);
void    print_export(char *env);
int  pwd(t_command *cmd, char **env);
void unset(t_command *cmd, t_env **env);
void unset_helper(char *cmd, char **env, int len);
char *get_env_value(char *name);
size_t length(char *s);

//execution
void execute_single_cmd(t_command *input, char **env);
execute_cmd(char **cmd, char **env );
void execute_builtin(t_command *cmd, char **env, int index);
char *get_path(char **cmd);
char *check_path(char **cmd, char **path);
char **ft_free(char **str);

//redirection
void    ft_redict(t_command *cmd, char **env);
void redic_not_builtin(t_command *cmd, char **env);
void redic_builtin(t_command *cmd, char **env);
void exec_in_child(t_command *cmd, char **env);
int get_in_v2(t_command *tmp, int fd_in, int index);
int get_out(t_command *tmp, int fd_out);
int get_in(t_command *tmp, int fd_in);
void restore_fd(int in, int out, int new_in, int new_out);
void dup_in_out(int in, int out);


#endif
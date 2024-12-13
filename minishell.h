/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iabboudi <iabboudi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 12:10:05 by stakhtou          #+#    #+#             */
/*   Updated: 2024/12/13 01:35:12 by iabboudi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "1337Libft/libft.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termios.h>
# include <unistd.h>
# define NOT_BUILT_IN -1
# include <readline/readline.h>
//# include "Garbage_collector/header.h"
typedef struct s_memref
{
	void					*mem_data;
	struct s_memref			*next;
}							t_memref;

typedef struct s_memgroup
{
	int						id;
	t_memref				*mem_refs;
	struct s_memgroup		*next;
}							t_memgroup;
t_memgroup					**gc_get_memgroups(void);
t_memgroup					*gc_create_mem_group(int id);
t_memgroup					*gc_get_specific_memgroup(int id);
t_memref					**gc_get_memrefs(int id);
void						gc_free_memrefs(t_memref *mem_ref);
void						gc_free_specific_memref(t_memref **mem_ref_head,
								t_memref *mem_ref_to_free);
void						gc_add(int mem_group_id, void *mem);
void						gc_add_double(int mem_group_id, void **mem);
void						gc_free_memgrp(int mem_group_id);
void						gc_free_all(void);
void						exit_minishell(int exit_code);

# define P 0644
# define PATH_MAX 4096

typedef struct s_global_vars
{
	unsigned char			exit_status;
	char					**env;
	int						heredoc_interrupted;
	int						khbi;
	int						in_pipe;
	char					*current_dir;
	char					*saved_oldpwd;
	int						flag_check;
	int						in_fd;
	int						env_allocated;
	int						error_printed;
	int						env_locked;

}							t_global_vars;

extern t_global_vars		g_vars;

typedef enum s_token_type
{
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
}							t_token_type;

typedef struct s_token
{
	int						type;
	int						quoted;
	char					*value;
	int						space;
	int						is_env_var;
	struct s_token			*next;

}							t_token;

typedef struct s_append
{
	int						i;
	char					*var_name;
	char					*var_value;
	char					*plus_pos;
	char					*equals_pos;
	char					*new_value;
	char					*temp;
}							t_append;

typedef struct s_command
{
	char					*name;
	char					**args;
	int						arg_count;
	int						pipe_next;
	struct s_redirection	*redirections;
	struct s_command		*next;
	struct t_pipe_list		*pipe_data;
}							t_command;

typedef struct s_pipe_list
{
	int						pipe_count;
	pid_t					*pids;
	int						pipes[2][2];
	int						i;
	t_command				*current;
	int						in_fd;
	int						out_fd;
}							t_pipe_list;

typedef struct s_handle_vars
{
	int						start;
	int						in_single_quotes;
	int						in_double_quotes;
	int						escaped;
	char					*value;
	t_token_type			type;
	t_token					*new;
	t_token					*last_token;
	char					*expanded_value;
	char					*final_value;
}							t_handle_vars;
typedef struct s_quote_state
{
	int						i;
	int						j;
	int						in_double_quotes;
	int						in_single_quotes;
}							t_quote_state;

typedef struct s_lexer_state
{
	t_token					*tokens;
	int						i;
	int						len;
	int						expect_heredoc_delim;
	int						expect_filename;
	char					current_char;
	char					next_char;
}							t_lexer_state;

typedef struct s_expansion
{
	char					*result;
	char					*temp;
	char					*new_result;
	char					*before_env;
	char					*env_pos;
	char					*env_name;
	char					*env_value;
	int						env_len;
	int						in_single_quote;
	int						in_double_quote;
	char					*final_result;
	char					*unquoted_result;
}							t_expansion;
typedef struct s_heredoc
{
	const char				*delimiter;
	char					*unquoted_delimiter;
	int						is_quoted;
	char					*line;
	char					*content;
	size_t					content_size;
	size_t					content_capacity;
	char					*processed_line;
	size_t					line_len;
	int						expand_vars;
}							t_heredoc;

typedef struct s_redirection
{
	int						type;
	char					*filename;
	struct s_redirection	*next;
	int						new_fd;
	int						output_fd;
	char					buffer[4096];
	ssize_t					bytes_read;

}							t_redirection;

typedef struct s_tokenizer_params
{
	const char				*input;
	int						i;
	int						len;
	int						expect_command;
	int						expect_filename;
	t_token					**tokens;
}							t_tokenizer_params;

typedef struct s_heredoc_manager
{
	pid_t					*heredoc_pids;
	int						heredoc_count;
}							t_heredoc_manager;

typedef struct s_redirection_chars
{
	char					current_char;
	char					next_char;
}							t_redirection_chars;

typedef struct s_quote_info
{
	int						len;
	char					quote_char;
}							t_quote_info;
typedef struct s_var_expansion
{
	char					*var_start;
	char					*var_end;
	size_t					var_name_len;
	char					*var_name;
	const char				*var_value;
	size_t					value_len;
}							t_var_expansion;

typedef struct s_handle_quotes_params
{
	const char				*input;
	int						*i;
	t_quote_info			quote_info;
	t_token					**tokens;
}							t_handle_quotes_params;
typedef struct s_pipe_data
{
	int						pipe_count;
	pid_t					*pids;
	int						pipes[2][2];
	int						i;
	t_command				*current;
	int						in_fd;
	int						out_fd;
	int						*prev_pipe;
	int						*curr_pipe;
}							t_pipe_data;

typedef struct s_expand_vars
{
	char					*result;
	char					*temp;
	char					*env_pos;
	char					*before_env;
	char					*env_value;
	char					*new_result;
	char					*final_result;
	char					*unquoted_result;
}							t_expand_vars;

typedef struct s_quote_result
{
	char					*quoted;
	char					*result;
	char					*final_result;
}							t_quote_result;

typedef struct s_quote_vars
{
	int						len;
	char					*result;
	int						j;
	int						in_double_quotes;
	int						in_single_quotes;
	int						i;
}							t_quote_vars;

typedef struct s_parse_context
{
	t_command		*command_list;
	t_command		*current_command;
	int				status;
	char			*env_value;
	char			*heredoc_content;
	char			temp_filename[sizeof("/tmp/minishell_heredocXXXXXX")];
	int				fd;
	char			exit_status_str[12];
}					t_parse_context;

typedef struct s_env_var_data
{
	char					*env_value;
	char					**split_value;
	int						j;
	t_token					*new;
}							t_env_var_data;
typedef struct s_expv
{
	size_t					result_len;
	size_t					result_cap;
	char					*result;
	char					*p;
}							t_expv;
void						init_shell(char **env);
void						sigint_handler(int sig);
void						sigquit_handler(int sig);
void						all_signals(void);
int							is_quoted(const char *str);

t_token						*new_token(int type, const char *value);
void						*ft_realloc(void *ptr, size_t size);
void						pipe_signals(void);

void						add_token(t_token **head, t_token *new_token);
void						*ft_calloc(size_t count, size_t size);
t_token						*tokenize_input(const char *input);
int							handle_quotes(const char *input, int *i,
								t_quote_info quote_info, t_token **tokens);
char						*remove_quotes(const char *str);
void						*ft_realloc(void *ptr, size_t size);

void						print_tokens(t_token *tokens);
int							calculate_quote_num(const char *input, int len,
								int *j, int *p);
int							is_valid_delimiter(const char *delimiter);
char						*get_directory_path(const char *filename);
int							check_directory(const char *path, int check_write);
int							check_file(const char *path, int mode);

void						create_new_var(t_append *append, char ***env);
void						cleanup_append(t_append *append);
int							handle_existing_var(t_append *append, char ***env);
void						append_export(char *cmd, char ***env, int len);

void						init_append(t_append *append, char *cmd);
int							is_var_match(char *env_var, char *var_name);
void						handle_existing_with_value(t_append *append,
								char ***env);
void						handle_existing_no_value(t_append *append,
								char ***env);

int							my_mkstemp(char *template);
char						*expand_variables(const char *str);
void						increment_shlvl(char **env, bool is_unset);
void						append_export(char *cmd, char ***env, int len);

int							ft_isspace(char c);
void						handle_command_or_argument(const char *input,
								int *i, int len, t_token **tokens);
void						free_tokens(t_token *head);
void						update_current_and_next_char(t_lexer_state *state,
								const char *input);
void						initialize_state(t_lexer_state *state,
								const char *input);
int							handle_heredoc_cases(t_lexer_state *state,
								const char *input);
int							handle_whitespace(t_lexer_state *state);
void						update_env_variable(char **env, char *var,
								char *value);
void						update_wds(char **env, const char *wd);
bool						chdir_errno_mod(const char *path);
bool						change_dir(char **env, char *path);
void						handle_tilde(char **env, char **path);
void						cd(t_command *cmd);
int							handle_pipe(t_lexer_state *state);
void						reset_after_heredoc(void);
t_command					*new_command(void);
int							myrand(void);
int							create_temp_file(char *template);
int							my_mkstemp(char *template);
char						*allocate_result(size_t capacity);
char						*extract_var_name(char **p);
char						*expand_variable(char **result, size_t *result_len,
								size_t *result_cap, const char *var_value);
char						*append_char(char *result, size_t *result_len,
								size_t *result_cap, char c);
char						*expand_env_vars(char *input);
char						*handle_variable_expansion(char **p, char *result,
								size_t *result_len, size_t *result_cap);
void						sigint_handlerh(int sig);
void						initialize_heredoc(t_heredoc *hdoc,
								const char *delimiter);
int							process_line(t_heredoc *hd);
int							resize_content_if_needed(t_heredoc *hdoc);
void						append_line_to_content(t_heredoc *hdoc);
void						heredoc_signals(void);

void						reset_signals(void);
char						*handle_heredoc(const char *delimiter,
								int expand_vars);
int							read_and_process_line(t_heredoc *hdoc,
								int expand_vars);
void						cleanup_heredoc(t_heredoc *hdoc);
int							read_and_process_line(t_heredoc *hdoc,
								int expand_vars);
void						add_argument(t_command *cmd, char *arg);
void						add_redirection(t_command *cmd, int type,
								char *filename);
char						*read_from_pipe(int fd);
void						write_and_free(int pipe_fd[2], t_heredoc *hd,
								char *result);

void						add_command(t_command **list, t_command *cmd);
char						*ft_strjoin_char(char *s, char c);
char						*process_quotes(t_expansion *exp);
int							read_and_process_line(t_heredoc *hdoc,
								int expand_vars);
t_command					*parse_tokens(t_token *tokens);
void						free_command(t_command *cmd);
int							get_status(void);
int							validate_syntax(t_token *tokens);
char						*remove_quotes(const char *str);
char						*remove_single_quotes(const char *str);
char						*ft_strndup(const char *s, size_t n);
void						handlee_heredoc(int *i, t_token **tokens);
void						handle_heredoc_delim(const char *input, int *i,
								int len, t_token **tokens);
void						handle_redirections(int *i,
								t_redirection_chars chars, t_token **tokens,
								int *expect_filename);
void						handle_filename(const char *input, int *i, int len,
								t_token **tokens);
void						handle_env_var(const char *input, int *i, int len,
								t_token **tokens);
void						sigint_handler(int sig);
void						setup_terminal(void);
void						all_signals(void);
void						pipe_signals(void);
void						child_signals(void);
void						sigint_handlerh(int signum);
char						**ft_free(char **str);
char						*check_path(char **cmd, char **path);
char						*plo(char **cmd);
char						*get_path(char **cmd);
void						execute_child(char **cmd, char *path);
void						execute_cmd(char **cmd);
void						exec(t_command *cmd);
void						execute_single_cmd(t_command *cmd, char **env);
void						concatinate(t_token **tokens);
int							check_heredoc_delim(t_token *tokens);
void						free_command_list(t_command *list);
void						print_command_list(t_command *list);
char						*expand_env_vars(char *input);
char						*expand_single_var(char **p, char *result,
								size_t *result_len, size_t *result_cap);
char						*realloc_result_buffer(char *result,
								size_t new_cap);
char						*copy_char_to_result(char *result,
								size_t *result_len, size_t *result_cap, char c);
char						*remove_enclosing_quotes(char *str);
void						parse_token_one(t_parse_context *ctx,
								t_token **tokens);
void						parse_token_two(t_parse_context *ctx,
								t_token **tokens);
void						parse_token_three(t_parse_context *ctx,
								t_token **tokens);
void						parse_token_four(t_parse_context *ctx,
								t_token **tokens);
void						parse_token_five(t_parse_context *ctx,
								t_token **tokens);
int							myrand(void);
void						execute_external_command(t_command *current,
								char **env);
int							create_temp_file(char *template);
int							my_mkstemp(char *template);
char						*expand_env_vars(char *input);
void						sigint_handlerh(int sig);

pid_t						execute_piped_command(t_command *cmd, int in_fd,
								int out_fd, char **env);
void						update_old_pwd(char *old_pwd, char **env);
void						update_pwd(char *pwd, char **env);
char						*ft_chr(char **env, char *vrb);
int							echo(t_command *cmd, char **env);
int							first_non_option(char **args);
int							is_n_option(char *arg);
void						env(t_command *cmd);
void						handle_pipes(t_command *commands, char **env);
void						setup_child_signals(void);
void						close_unused_fds(int red_in, int red_out, int in_fd,
								int out_fd);
void						execute_command(t_command *cmd, char **env);
int							check_file_path(const char *filename,
								int creating_file);
pid_t						execute_piped_command(t_command *cmd, int in_fd,
								int out_fd, char **env);
char						*get_directory_path(const char *filename);

int							count_pipes(t_command *commands);
void						setup_pipe(int pipes[2][2], int i);
void						determine_fds(int *in_fd, int *out_fd, int in_pipe,
								int out_pipe);
void						close_pipe_fds(int i, t_command *current,
								int pipes[2][2]);
void						wait_for_children(pid_t *pids, int pipe_count);
void						setup_redirections_v2(t_command *cmd, int in_fd,
								int out_fd);
void						handle_pipes(t_command *commands, char **env);
void						add_to_env(char ***env, char *new_var);
int							double_pointer_len(char **str);
void						ft_setter(int value);
int							ft_getter(void);
int							is_builtin(t_command *cmd);
void						setup_redirection(t_command *cmd);
int							is_num(char *str);
void						export(t_command *cmd);
int							check_export(char *cmd);
int							export_helper(char *cmd, char ***env, int len);
void						print_export(char *env);
int							pwd(t_command *cmd, char **env);
void						unset(t_command *cmd);
int							check_env(char *cmd, char **env);
void						unset_helper(char *cmd, int len);
char						*get_env_value(char *name, char **env);
void						add_to_envp(char ***env, char *new_var);
size_t						length(char *s);
void						execute_single_cmd(t_command *input, char **env);
void						execute_cmd(char **cmd);
void						execute_builtin(t_command *cmd, char **env,
								int index);
char						*get_path(char **cmd);
char						*check_path(char **cmd, char **path);
char						**ft_free(char **str);
void						ft_redict(t_command *cmd, char **env);
void						redic_not_builtin(t_command *cmd, char **env);
void						redic_builtin(t_command *cmd, char **env);
void						exec_in_child(t_command *cmd, char **env);
int							get_in_v2(t_command *tmp, int fd_in, int index);
void						restore_fd(int in, int out, int new_in,
								int new_out);
int							get_out(t_command *cmd, int fd_out);
int							get_in(t_redirection *red, int fd_in);
void						setup_redirections_v2(t_command *cmd, int in_fd,
								int out_fd);

char						**create_env(void);
void						dup_in_out(int in, int out);
void						handle_exit_status(int status);
void						init_current_dir(void);
char						*build_path(char *base, char *path);
char						*get_current_dir(void);
void						update_current_dir(char *new_path);
void						close_prev_pipe(int prev_pipe[2]);
void						update_prev_pipe(int prev_pipe[2],
								int curr_pipe[2]);
void						wait_for_children_and_cleanup(t_pipe_data *data);
void						execute_external_command(t_command *current,
								char **env);
void						setup_child_process(int prev_pipe[2],
								int curr_pipe[2], t_command *current,
								char **env);
void						handle_pipes_in_child(int prev_pipe[2],
								int curr_pipe[2]);
bool						setup_next_pipe(int curr_pipe[2],
								t_command *current);
bool						initialize_pipe_data(t_pipe_data *data,
								t_command *commands);
void						handle_command_execution(t_command *current,
								char **env);

#endif
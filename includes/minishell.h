/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+
	+:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+
	+#+           */
/*   Created: 2025/04/24 16:15:02 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/27 10:45:00 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../Libft/libft.h"
# include "memory.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
# include <sys/ioctl.h>

/* Defines */
# define _GNU_SOURCE
# define LMAX "9223372036854775808"
# define LMIN "-9223372036854775809"
# define SUCCESS 0
# define FAILURE 1
# define PROMPT "minishell-> "
# define READ_END 0
# define WRITE_END 1
# define HERE_TEMP "/tmp/heredoc_XXXXXX"
# define MAX_TOKENS 1024
# define RESET_COLOR "\033[0m"
# define PROMPT_COLOR "\033[1;32m"
# define ERROR_COLOR "\033[1;31m"
# define MAX_ENV_VAR_SIZE 1048576

typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}	t_redir_type;

typedef struct s_lexer
{
	char	*start;
	bool	in_quote;
	char	quote_char;
	int		tokcount;
}	t_lexer;

typedef struct s_redir
{
	t_redir_type	type;
	char			*file;
	char			*prefile;
	struct s_redir	*next;
}	t_redir;

typedef struct s_expander_state
{
	char	*result;
	char	*dst;
	char	quote_char;
	bool	in_quote;
	bool	in_dquote;
	bool	is_heredoc;
}	t_expander_state;

typedef struct s_split_context
{
	char	*expanded;
	char	**new_args;
	int		*count;
	int		arg_index;
}	t_split_context;

typedef struct s_cmd
{
	int				in_fd;
	int				out_fd;
	char			*in_file;
	pid_t			*pid_array;
	char			*out_file;
	char			**args;
	t_redir			*redirs;
	bool			syntax_error;
	struct s_cmd	*next;
	struct s_shell	*shell;
	int				exit_status;
	int				alloc_count;
}	t_cmd;

typedef struct s_shell
{
	bool	path_unset;
	char	**env;
	int		exit_status;
	bool	is_interactive;
	int		signal_status;
	t_cmd	*cmd;
}	t_shell;

extern volatile sig_atomic_t	g_signal_received;

void	safe_free(void *ptr);
void	print_error_message(const char *prefix, char *msg, char *extra);
void	free_tokens(char **tokens);
void	free_ptr_array(void **array);

bool	contains_metacharacter(char *token);
int		handle_quotes(char **input, bool *in_quote, char *quote_char);
char	**lexer(char *input);
int		count_tokens(char *input);
bool	is_metacharacter(char *token);
bool	contains_metacharacter(char *token);

/* Parser */
int		handle_input(t_shell *shell, char *input);
void	finish_cd_success(t_cmd *cmd, char *old_pwd, char *path_dup);
int		check_redirect_filename(char **tokens,
			int *i, t_shell *shell, t_cmd *cmd);
void	create_redir_node(t_cmd *cmd, int type, char *file);
t_cmd	*parser(char **tokens, t_shell *shell);
t_cmd	*create_cmd_node(void);
int		is_redirection(char *tokens);
void	handle_parser_token(t_cmd **cmd, char **tokens, int *i,
			t_shell *shell);
int		check_pipe_errors(char **tokens, t_shell *shell, int i);
int		check_redir_errors(char **tokens, t_shell *shell, int *i);
void	handle_syntax_error(char *token, t_shell *shell);
int		same_length(char *tok1, char *tok2);

/* Command Handling */
void	find_and_update_env(char *arg, char *var_name, char ***env);
void	handle_var_size_error(char *var_name);
int		var_is_too_large(char *arg);
void	initialize_shell(t_shell *shell, char **argv);
void	handle_heredoc_direct(t_cmd *cmd, char **tokens, int *i, \
		t_shell *shell);
void	handle_pipeline(t_cmd **current, t_shell *shell);
void	handle_pipe_token(t_cmd **current, char **tokens, int *i, \
			t_shell *shell);
void	handle_heredoc(t_cmd *cmd, char *eof);
int		handle_plus_syntax(char *arg, char ***env);

/* Execution */
char	*resolve_path_parts(char **parts, int count);
char	*build_full_path(const char *current_pwd, const char *path);
void	executor(t_cmd *cmd, t_shell *shell);
void	execute_single_command(t_cmd *cmd, t_shell *shell);
void	execute_external_command(t_cmd *cmd, t_shell *shell);
void	waitpid_for_single_command(pid_t pid, t_shell *shell);
void	execute_pipeline(t_cmd *cmd, t_shell *shell);
void	execve_error(t_cmd *cmd);
char	*get_clean_cmd_name(const char *path);
int		fork_pipeline_commands(t_cmd *cmd, t_shell *shell, pid_t *pids, \
			pid_t *last_pid);
pid_t	fork_child_process(t_cmd *cmd, int prev_pipe_in, int pipe_fd[2], \
						t_shell *shell);
void	close_pipe_exec(int pipe_fd[2], int ppi);
pid_t	*allocate_pid_array(int count);
int		count_pipeline_cmds(t_cmd *cmd);
void	wait_for_pipeline(pid_t *pids, int count, pid_t last_pid,
			t_shell *shell);
void	handle_fork_error(pid_t *pids);
void	rearrange_pipes(t_cmd *cmd, int prev_pipe_in, int pipe_fd[2]);
void	setup_parent_after_fork(t_cmd *cmd, int *prev_pipe_in, int pipe_fd[2]);

/* Redirections */
void	close_cmd_fds(t_cmd *cmd);
bool	apply_redirection(t_cmd *cmd, bool fork);
void	handle_redirection_out_append(t_cmd *cmd, t_redir *redir);
bool	handle_redirection_out(int append, t_redir *redir, t_cmd *cmd,
			bool fork);
bool	handle_redirection_in(t_redir *redir, t_cmd *cmd, bool fork);
char	*process_da_bullshit(char *arg, t_shell *shell);
void	forked_exit(int code, t_cmd *cmd);

/* Builtins */
void	handle_cd_path(t_cmd *cmd, char **path, char **path_dup,
			char *old_pwd);
void	execute_builtin(t_cmd *cmd);
int		is_builtin(char *cmd);
void	ft_echo(t_cmd *cmd);
void	ft_cd(t_cmd *cmd);
void	ft_pwd(t_cmd *cmd);
void	ft_export(t_cmd *cmd);
void	ft_unset(t_cmd *cmd);
void	ft_env(t_cmd *cmd);
void	ft_exit(t_cmd *cmd);

/* Signals */
void	setup_signals(void);
void	reset_signals(void);
void	ignore_signals(void);
void	handle_sigint(int sig);
void	handle_sigquit(int sig);
void	setup_heredoc_signals(void);

/* Environment */
void	update_logical_pwd(t_shell *shell, const char *path);
char	*calculate_logical_path(const char *current_pwd, const char *path);
char	*resolve_path_in_current_dir(char *cmd);
void	resolve_non_builtin_path(t_cmd *node, t_shell *shell);
int		handle_dollar_sign(char *arg, int i, t_expander_state *state,
			t_shell *shell);
char	*handle_append_syntax(char *arg, char **env);
char	*extract_var_name(char *arg);
char	*resolve_path(char *cmd, char **env);
void	update_or_add_env(char *arg, char ***env);
void	add_to_env(char ***env, char *new_var);
char	**copy_env(char **env);
char	*get_env_value(char *name, char **env);
void	set_env_value(char *key, char *value, t_shell *shell);
void	ft_sort_strings(char **arr, int count);
void	print_sorted_env(char **env);
void	update_pwd(t_shell *shell);
void	update_shlvl(t_shell *shell);
int		is_valid_key(char *key);
void	print_sorted_env(char **env);
void	free_env_copy(char **env_copy);
void	print_env_var(char *env_var, int fd);
char	**copy_env_array(char **env, int env_count);

/* Utils */
int		handle_fork_failure(int prev_pipe_in, t_cmd *node, pid_t *pids, \
	int *pipe_fd);
int		unclosed_check(char *input);
void	cd_error(char *path_dup);
char	*update_prompt(void);
bool	is_special_char(char c);
int		ft_isspace(char c);
int		is_redir_without_space(char **input);
void	ft_free(char **arr);
void	free_cmd(t_cmd *cmd);
void	free_tokens(char **tokens);
void	free_env(char **env);
void	free_redir(t_redir *redir);
void	print_error(char *msg, char *arg);
bool	is_quoted(char *token);
bool	contains_metacharacter(char *token);
void	free_shell(t_shell *shell);
char	*append_str(char *dest, char *src);
bool	is_metacharacter(char *token);
void	add_argument(t_cmd *node, char *arg);
void	update_shlvl(t_shell *shell);
int		is_env_cmd(char *cmd);
int		count_special_char(char *input);
void	handle_signal_exit(t_shell *shell);
int		process_shell_input(t_shell *shell, char *input);
bool	needs_word_splitting(char *str);

/* Memory Management */
void	cleanup_shell(t_shell *shell);
void	*xmalloc(size_t size);
void	safe_free(void *ptr);

/* Here Document */
int		handle_heredoc_redirect(t_cmd *cmd, char *clean_eof, \
		char *temp_name, bool expand_vars);
int		handle_heredoc_fork(t_cmd *cmd, char *clean_eof, char *temp_name, \
		bool expand_vars);
void	read_heredoc_input(const char *delimiter, int fd, t_cmd *cmd,
			bool expand_vars);
char	*handle_expanded_line(char *arg, char *expanded_line);
char	*remove_quotes(char *lim);
void	cleanup_heredocs(t_shell *shell);
char	*get_random_temp_name(void);
char	*clean_empty_expansions(char *arg);
bool	check_delimiter_quotes(char *eof);

/* Variable Expansion */
bool	is_export_var_assignment(char **args, int arg_index);
int		count_args_after_splitting(char **args, t_shell *shell);
char	*handle_quoted_content(char *str, int start, int *i);
char	*safe_strjoin(char *s1, char *s2, int free_flag);
char	**split_expanded_variable(char *value);
char	*process_argument(char *arg, t_shell *shell);
char	*expand_variable(char *arg, int *i, t_shell *shell);
void	expander(t_cmd *cmd, t_shell *shell);
void	expand_nodes(t_cmd *cmd, t_shell *shell);
char	**apply_word_splitting(char **args, t_shell *shell);
void	handle_special_dir(t_cmd *cmd, t_shell *shell, char *error_msg,
			int exit_code);

/* Main */
void	minishell_loop(t_shell *shell);
void	initialize_shell(t_shell *shell, char **argv);
int		handle_input(t_shell *shell, char *input);
int		main(int argc, char **argv, char **envp);

#endif

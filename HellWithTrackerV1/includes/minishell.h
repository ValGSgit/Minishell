#ifndef MINISHELL_H
# define MINISHELL_H

# include "../Libft/libft.h"
# include "malloc_tracker.h"
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

/* Memory Management Function Declarations */
void	*xmalloc(size_t size);
void	xfree(void *ptr);
void	clean_memory(void);

/* Enums */
typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}	t_redir_type;

/* Structs */
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

typedef struct s_cmd
{
	int				in_fd;
	char			*in_file;
	int				out_fd;
	char			*out_file;
	char			**args;
	t_redir			*redirs;
	char			**env;
	bool			syntax_error;
	struct s_cmd	*next;
	struct s_shell	*shell;
	int				exit_status;
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

/* Function Prototypes */

/* Debug */
void	debug_shell_state(char **tokens, t_cmd *cmd, char *stage);

/* Main Loop */
void	update_pwd(t_shell *shell);
void	minishell_loop(t_shell *shell);

/* Lexer */
char	**lexer(char *input);
int		handle_quotes(char **input, bool *in_quote, char *quote_char);

/* Parser */
void	handle_heredoc_redirect(t_cmd *cmd, char **tokens, int *i,
			t_shell *shell);
int		same_length(char *tok1, char *tok2);
void	handle_parser_token(t_cmd **current, char **tokens, int *i,
			t_shell *shell);
void	handle_pipe_token(t_cmd **current, char **tokens, int *i,
			t_shell *shell);
void	handle_redirection(t_cmd *cmd, char **tokens, int *i, t_shell *shell);
int		is_redirection(char *tokens);
void	handle_syntax_error(char *token, t_shell *shell);
int		check_syntax_errors(char **tokens, t_shell *shell);
int		check_pipe_errors(char **tokens, t_shell *shell, int i);
int		check_redirect_filename(char **tokens, int *i, t_shell *shell, t_cmd *cmd);
int		check_quotes(char *token, t_shell *shell);
int		check_redir_errors(char **tokens, t_shell *shell, int *i);
t_cmd	*parser(char **tokens, t_shell *shell);

/* Heredocs */
char	*ft_strjoin_free(char *s1, char *s2);
char	*ft_strjoin_char(char *str, char c);
bool	check_delimiter_quotes(char *eof);
void	setup_heredoc_signals(void);
char	*handle_expanded_line(char *arg, char *expanded_line);
void	read_heredoc_input(const char *delimiter, int fd, t_cmd *cmd,
			bool expand_vars);
void	heredoc_child_process(t_cmd *cmd, char *clean_eof, int fd,
			bool expand_vars);
int		heredoc_parent_process(pid_t pid, t_cmd *cmd, int fd, char *clean_eof);
char	*remove_quotes(char *lim);
char	*process_line(char *arg, t_cmd *cmd, bool expand_vars);
char	*clean_empty_expansions(char *arg);
const char	*get_random_temp_name(void);
int	open_heredoc_file(const char *temp_name, char *clean_eof);
void	handle_heredoc_fork(t_cmd *cmd, char *clean_eof, int fd, bool expand_vars);

/* Expander */
char	**apply_word_splitting(char **args, t_shell *shell);
void	handle_pipeline(t_cmd **current, t_shell *shell);
char	**split_expanded_variable(char *value);
char	*process_argument(char *arg, t_shell *shell);
char	*expand_variable(char *arg, int *i, t_shell *shell);
void	expand_nodes(t_cmd *cmd, t_shell *shell);
void	expander(t_cmd *cmd, t_shell *shell);
char	*resolve_path(char *cmd, char **env);

/* Executor */
void	executor(t_cmd *cmd, t_shell *shell);
void	only_redir_fork(t_cmd *cmd);
void	execute_builtin_or_exit(t_cmd *cmd);
int		handle_only_redirection(t_cmd *cmd);
void	waitpid_for_single_command(pid_t pid, t_shell *shell);
void	execve_error(t_cmd *cmd);
void	external_cmd_checks(t_cmd *cmd);
void	execute_external_command(t_cmd *cmd, t_shell *shell);
void	execute_single_command(t_cmd *cmd, t_shell *shell);
char	*get_clean_cmd_name(const char *path);
char	*get_cmd_path(char *cmd, t_shell *shell);
void	execute_command(t_cmd *cmd);
void	handle_exec_error(t_cmd *cmd, char *cmd_path);
int		execute_child(t_cmd *cmd);

/* Pipes */
void	execute_pipeline(t_cmd *cmd, t_shell *shell);
pid_t	fork_child_process(t_cmd *cmd, int prev_pipe_in, int pipe_fd[2],
			t_shell *shell);
void	setup_parent_after_fork(t_cmd *cmd, int *prev_pipe_in, int pipe_fd[2]);
void	wait_for_pipeline(pid_t *pids, int count, pid_t last_pid,
			t_shell *shell);
void	rearrange_pipes(t_cmd *cmd, int prev_pipe_in, int pipe_fd[2]);
int		fork_pipeline_commands(t_cmd *cmd, t_shell *shell, pid_t *pids,
			pid_t *last_pid);
void	handle_fork_error(pid_t *pids);
pid_t	*allocate_pid_array(int count);
int		count_pipeline_cmds(t_cmd *cmd);

/* Redirections */
t_cmd	*create_cmd_node(void);
void	create_redir_node(t_cmd *cmd, int type, char *file);
void	apply_redirection(t_cmd *cmd);
void	handle_heredoc(t_cmd *cmd, char *eof);
void	handle_redirection_out(t_redir *redir, int append, t_cmd *cmd);
void	handle_redirection_in(t_redir *redir, t_cmd *cmd);

/* Builtins */
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

/* Environment */
void	update_or_add_env(char *arg, char ***env);
void	add_to_env(char ***env, char *new_var);
char	**copy_env(char **env);
char	*get_env_value(char *name, char **env);
void	set_env_value(char *key, char *value, t_shell *shell);
void	ft_sort_strings(char **arr, int count);

/* Export Functions */
void	print_sorted_env(char **env);
void	sort_env(char **env_copy, int env_count);
void	swap_strings(char **a, char **b);
int		is_valid_identifier(char *str);
char	*extract_var_name(char *arg);
char	*handle_append_syntax(char *arg, char **env);

/* Utils */
char	*update_prompt(void);
bool	is_special_char(char c);
int		ft_isspace(char c);
int		is_redir_without_space(char **input);
void	ft_free(char **arr);
void	free_cmd(t_cmd *cmd);
void	free_tokens(char **tokens);
void	free_env(char **env);
void	print_error(char *msg, char *arg);
bool	is_quoted(char *token);
bool	contains_metacharacter(char *token);
void	free_shell(t_shell *shell);
char	*append_str(char *dest, char *src);
bool	is_metacharacter(char *token);
void	add_argument(t_cmd *node, char *arg);
void	update_shlvl(t_shell *shell);

#endif

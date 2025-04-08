/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 12:01:26 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/08 15:25:59 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef MINISHELL_H
# define MINISHELL_H

# include "../ShellLibft/libft.h"
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

/* Enums */
typedef enum e_redir_type {
    REDIR_IN,     // <
    REDIR_OUT,    // >
    REDIR_APPEND, // >>
    REDIR_HEREDOC // <<
}	t_redir_type;

/* Structs */
typedef struct s_lexer {
    char	*start;
    bool	in_quote;
    char	quote_char;
    int		tokcount;
}	t_lexer;

typedef struct s_redir {
    t_redir_type	type;
    char			*file;
    struct s_redir	*next;
}	t_redir;

typedef struct s_expander_state {
    char	*result;
    char	*dst;
    char	quote_char;
    bool	in_quote;
    bool	in_dquote;
}	t_expander_state;

typedef struct s_cmd {
    int				in_fd;
    char			*in_file;
    int				out_fd;
    char			*out_file;
    char			**args;
    t_redir			*redirs;
    char			**env;
   // int				exit_status;
    bool			syntax_error;
    struct s_cmd	*next;
    struct s_shell	*shell;
}	t_cmd;

typedef struct s_shell {
    char	**env;
    int		exit_status;
    bool	is_interactive;
    int		signal_status;
    t_cmd	*cmd;
}	t_shell;

/* Function Prototypes */

/* Debug */
void	debug_shell_state(char **tokens, t_cmd *cmd, const char *stage);

/* Main Loop */
void	minishell_loop(t_shell *shell);

/* Lexer */
char	**lexer(char *input);
int		handle_quotes(char **input, bool *in_quote, char *quote_char);

/* Parser */
t_cmd	*parser(char **tokens, t_shell *shell);

/* Expander */
char	*process_argument(char *arg, t_shell *shell);
char	*expand_variable(char *arg, int *i, t_shell *shell, bool in_dquote);
void	expand_nodes(t_cmd *cmd, t_shell *shell);
void	expander(t_cmd *cmd, t_shell *shell);
char	*resolve_path(char *cmd, char **env);

/* Executor */
void	executor(t_cmd *cmd, t_shell *shell);

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

/* Redirections */
t_cmd	*create_cmd_node(void);
void	create_redir_node(t_cmd *cmd, int type, char *file);
void	restore_redirections(t_cmd *cmd);
void	apply_redirection(t_cmd *cmd);
void	handle_heredoc(t_cmd *cmd, char *eof);

/* Pipes */
void	execute_command(t_cmd *cmd, t_shell *shell);
void	handle_pipes(t_cmd *cmd);
void    pipe_exit_status(pid_t last_pid, t_shell *shell);
pid_t	fork_child_process(t_cmd *cmd, int prev_pipe_in, int pipe_fd[2], t_shell *shell);
void	setup_parent_after_fork(t_cmd *cmd, int *prev_pipe_in, int pipe_fd[2]);

/* Signals */
void	setup_signals(void);
void	reset_signals(void);
void	ignore_signals(void);
void	handle_sigint(int sig);
void	handle_sigquit(int sig);

/* Environment */
void	update_or_add_env(char *arg, char ***env);
void    add_to_env(char ***env, char *new_var);
char	**copy_env(char **env);
char	*get_env_value(char *name, char **env);
void	set_env_value(char *key, char *value, t_shell *shell);
void	ft_sort_strings(char **arr, int count);

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
void	free_shell(t_shell *shell);
char	*append_str(char *dest, char *src);
bool	is_metacharacter(char *token);
void	add_argument(t_cmd *node, char *arg);
void update_shlvl(t_shell *shell);

#endif
// #ifndef MINISHELL_H
// # define MINISHELL_H

// /******************************
//  *          INCLUDES          *
//  ******************************/

// # include "../ShellLibft/libft.h"
// # include <dirent.h>
// # include <errno.h>
// # include <fcntl.h>
// # include <limits.h>
// # include <readline/history.h>
// # include <readline/readline.h>
// # include <signal.h>
// # include <stdbool.h>
// # include <stdio.h>
// # include <stdlib.h>
// # include <string.h>
// # include <sys/stat.h>
// # include <sys/types.h>
// # include <sys/wait.h>
// # include <unistd.h>

// /******************************
//  *          DEFINES           *
//  ******************************/
// # define LMAX "9223372036854775808"
// # define LMIN "-9223372036854775809"
// # define SUCCESS 0
// # define FAILURE 1
// # define PROMPT "minishell-> "
// # define READ_END 0
// # define WRITE_END 1
// # define HERE_TEMP "/tmp/heredoc_XXXXXX"
// # define MAX_TOKENS 1024
// # define RESET_COLOR "\033[0m"
// # define PROMPT_COLOR "\033[1;32m" // Bright green
// # define ERROR_COLOR "\033[1;31m"  // Bright red

// /******************************
//  *          STRUCTS           *
//  ******************************/

// /* Redirection Types */
// typedef enum e_redir_type
// {
// 	REDIR_IN,     // <
// 	REDIR_OUT,    // >
// 	REDIR_APPEND, // >>
// 	REDIR_HEREDOC // <<
// }			t_redir_type;

// typedef struct s_lexer
// {
// 	char	*start;
// 	bool	in_quote;
// 	char	quote_char;
// 	int		tokcount;
// }			t_lexer;

// /* Redirection Node */
// typedef struct s_redir
// {
// 	t_redir_type type;    // Type of redirection
// 	char *file;           // File or delimiter for heredoc
// 	struct s_redir *next; // Next redirection in the list
// }			t_redir;

// typedef struct s_expander_state
// {
//     char    *result;
//     char    *dst;
//     char    quote_char;
//     bool    in_quote;
// 	bool    in_dquote;
// }   t_expander_state;

// /* Command Node */
// typedef struct s_cmd
// {
// 	int in_fd;       // Input file descriptor
// 	char *in_file;   // Input file name
// 	int out_fd;      // Output file descriptor
// 	char *out_file;  // Output file name
// 	char **args;     // Command arguments (e.g., ["ls", "-l"])
// 	t_redir	*redirs;
// 	char	**env;
// 	int		exit_status;
// 	bool	syntax_error;
// 	struct s_cmd *next; // Next command in the pipeline (e.g., cmd1 | cmd2)
// 	struct s_shell *shell;
// }			t_cmd;


// /* Shell State */
// typedef struct s_shell
// {
// 	char **env;          // Environment variables
// 	int exit_status;     // Exit status of the last command
// 	bool is_interactive; // Whether the shell is in interactive mode
// 	int signal_status;   // Global variable for signal handling
// 	struct s_cmd *cmd;   // Command to execute
// }			t_shell;

// /******************************
//  *      FUNCTION PROTOTYPES   *
//  ******************************/

// /* debug */
// void	debug_shell_state(char **tokens, t_cmd *cmd, const char *stage);

// /* Main Loop */
// void		minishell_loop(t_shell *shell);

// /* Lexer */
// char		**lexer(char *input, t_shell *shell);
// int			handle_quotes(char **input, bool *in_quote, char *quote_char);

// /* Parser */
// t_cmd		*parser(char **tokens, t_shell *shell);

// /* Expander */
// char	*process_argument(char *arg, t_shell *shell);

// char	*expand_variable(char *arg, int *i, t_shell *shell, bool in_dquote);
// void	expand_nodes(t_cmd *cmd, t_shell *shell);
// void		expander(t_cmd *cmd, t_shell *shell);

// /* Executor */
// void		executor(t_cmd *cmd, t_shell *shell);

// /* Builtins */
// void		execute_builtin(t_cmd *cmd);
// int			is_builtin(char *cmd);
// void		ft_echo(t_cmd *cmd);
// void		ft_cd(t_cmd *cmd);
// void		ft_pwd(t_cmd *cmd);
// void		ft_export(t_cmd *cmd);
// void		ft_unset(t_cmd *cmd);
// void		ft_env(t_cmd *cmd);
// void		ft_exit(t_cmd *cmd);

// /* Redirections */
// t_cmd		*create_cmd_node(void);
// // char	*expand_variable(char *arg, t_shell *shell);
// //void		add_redir_node(t_redir **head, t_redir *new_node);
// void		create_redir_node(t_cmd *cmd, int type, char *file);
// //void		handle_pipeline(t_cmd **cmd);
// //void		handle_redirections(t_cmd *cmd, char **tokens, int *i);
// void		restore_redirections(t_cmd *cmd);
// void		apply_redirection(t_cmd *cmd);

// /* Pipes */
// void		execute_command(t_cmd *cmd, t_shell *shell);
// // void		execute_pipeline(t_cmd *cmd, char **env);
// void		handle_pipes(t_cmd *cmd);

// /* Signals */
// void		setup_signals(void);
// void		reset_signals(void);
// void		ignore_signals(void);
// void		handle_sigint(int sig);
// void		handle_sigquit(int sig);

// /* Environment */
// void	update_or_add_env(char *arg, char **env);
// char		**copy_env(char **env);
// char		*get_env_value(char *name, char **env);
// void		set_env_value(char *key, char *value, t_shell *shell);
// void		ft_sort_strings(char **arr, int count);
// int			add_new_env_var(char *arg, char **env);

// /* Utils */
// char		*update_prompt(void);
// bool		is_special_char(char c);
// int			ft_isspace(char c);
// int			is_redir_without_space(char **input);
// void		ft_free(char **arr);
// void		free_cmd(t_cmd *cmd);
// void		free_tokens(char **tokens);
// void		free_env(char **env);
// void		print_error(char *msg, char *arg);
// bool		is_quoted(char *token);
// void	free_shell(t_shell *shell);
// char	*append_str(char *dest, char *src);

// #endif

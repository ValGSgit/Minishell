/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   planer.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 12:01:26 by vagarcia          #+#    #+#             */
/*   Updated: 2025/03/28 13:45:29 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/******************************
 *          INCLUDES          *
 ******************************/

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

/******************************
 *          DEFINES           *
 ******************************/
# define SUCCESS 0
# define FAILURE 1
# define PROMPT "minishell-> "
# define READ_END 0
# define WRITE_END 1
# define HERE_TEMP "/tmp/heredoc_XXXXXX"
# define MAX_TOKENS 1024
# define RESET_COLOR "\033[0m"
# define PROMPT_COLOR "\033[1;32m" // Bright green
# define ERROR_COLOR "\033[1;31m"  // Bright red

/******************************
 *          STRUCTS           *
 ******************************/

/* Redirection Types */
typedef enum e_redir_type
{
	REDIR_IN,     // <
	REDIR_OUT,    // >
	REDIR_APPEND, // >>
	REDIR_HEREDOC // <<
}					t_redir_type;

typedef struct s_lexer
{
	char			*start;
	bool			in_quote;
	char			quote_char;
	int				tokcount;
}					t_lexer;

/* Redirection Node */
typedef struct s_redir
{
	t_redir_type type;    // Type of redirection
	char *file;           // File or delimiter for heredoc
	struct s_redir *next; // Next redirection in the list
}					t_redir;

typedef struct s_expander_state
{
	char			*result;
	char			*dst;
	char			quote_char;
	bool			in_quote;
	bool			in_dquote;
}					t_expander_state;

/* Command Node */
typedef struct s_cmd
{
	int in_fd;      // Input file descriptor
	char *in_file;  // Input file name
	int out_fd;     // Output file descriptor
	char *out_file; // Output file name
	char **args;    // Command arguments (e.g., ["ls", "-l"])
	t_redir			*redirs;
	// t_redir *input;  // Input redirections (e.g., < file.txt)
	// t_redir *output; // Output redirections (e.g., > out.txt)
	char			**env;
	int				exit_status;
	bool			syntax_error;
	struct s_cmd *next; // Next command in the pipeline (e.g., cmd1 | cmd2)
	struct s_shell	*shell;
}					t_cmd;

/* Shell State */
typedef struct s_shell
{
	char **env;          // Environment variables
	int exit_status;     // Exit status of the last command
	bool is_interactive; // Whether the shell is in interactive mode
	int signal_status;   // Global variable for signal handling
	struct s_cmd *cmd;   // Command to execute
}					t_shell;

/******************************
 *      FUNCTION PROTOTYPES   *
 ******************************/

/* debug */
void				debug_shell_state(char **tokens, t_cmd *cmd,
						const char *stage);

/* Main Loop */
void				minishell_loop(t_shell *shell);

/* Lexer */
char				**lexer(char *input, t_shell *shell);

/* Parser */
t_cmd				*parser(char **tokens, t_shell *shell);

/* Expander */
void				expand_nodes(t_cmd *cmd, t_shell *shell);
void				expander(t_cmd *cmd, t_shell *shell);

/* Executor */
void				executor(t_cmd *cmd, t_shell *shell);

/* Builtins */
void				execute_builtin(t_cmd *cmd);
int					is_builtin(char *cmd);
void				ft_echo(t_cmd *cmd);
void				ft_cd(char **args, t_shell *shell);
void				ft_pwd(t_cmd *cmd);
void				ft_export(t_cmd *cmd);
void				ft_unset(t_cmd *cmd);
void				ft_env(t_cmd *cmd);
void				ft_exit(t_cmd *cmd);

/* Redirections */
t_cmd				*create_cmd_node(void);
void				create_redir_node(t_cmd *cmd, int type, char *file);
void				restore_redirections(t_cmd *cmd);
void				apply_redirection(t_cmd *cmd);

/* Pipes */
void				execute_command(t_cmd *cmd, t_shell *shell);
// void		execute_pipeline(t_cmd *cmd, char **env);
void				handle_pipes(t_cmd *cmd);

/* Signals */
void				setup_signals(void);
void				reset_signals(void);
void				ignore_signals(void);
void				handle_sigint(int sig);
void				handle_sigquit(int sig);

/* Environment */
void				update_or_add_env(char *arg, char **env);
char				**copy_env(char **env);
char				*get_env_value(char *name, char **env);
void				set_env_value(char *key, char *value, t_shell *shell);
void				ft_sort_strings(char **arr, int count);
int					add_new_env_var(char *arg, char **env);

/* Utils */
char				*update_prompt(void);
bool				is_special_char(char c);
int					ft_isspace(char c);
int					is_redir_without_space(char **input);
void				ft_free(char **arr);
void				free_cmd(t_cmd *cmd);
void				free_tokens(char **tokens);
void				free_env(char **env);
void				free_shell(t_shell *shell);
void				print_error(char *msg, char *arg);

#endif

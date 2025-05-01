/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 13:18:31 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/27 14:55:00 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _GNU_SOURCE
#include "../includes/minishell.h"


static int	s_heredoc_fd = -1;

void	heredoc_sigint(int sig)
{
	if (sig == SIGINT)
	{
		if (s_heredoc_fd != -1)
			close(s_heredoc_fd);
		exit(130);
	}
}

int	handle_heredoc_fork(t_cmd *cmd, char *clean_eof, char *temp_name, bool expand_vars)
{
	pid_t	pid;
	int		status;
	struct sigaction sa;

	pid = fork();
	if (pid == -1)
		return (1);

	if (pid == 0)
	{
		sa.sa_handler = heredoc_sigint;
		sigemptyset(&sa.sa_mask);
		sa.sa_flags = 0;
		sigaction(SIGINT, &sa, NULL);
		
		s_heredoc_fd = open(temp_name, O_CREAT | O_WRONLY | O_TRUNC, 0644); 
		if (s_heredoc_fd < 0)
			exit(EXIT_FAILURE); 
		read_heredoc_input(clean_eof, s_heredoc_fd, cmd, expand_vars);
		close(s_heredoc_fd);
		exit(EXIT_SUCCESS);
	}
	else
	{
		free(clean_eof);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
		{
			cmd->shell->exit_status = 130;
			g_signal_received = 130;
			cleanup_heredocs(cmd->shell);
			cmd->syntax_error = 1;
			return (130); // heredoc interrupted
		}
		else if (WIFSIGNALED(status))
		{
			cmd->shell->exit_status = 128 + WTERMSIG(status);
			g_signal_received = cmd->shell->exit_status;
			cleanup_heredocs(cmd->shell);
			write(1, "\n", 1);
			rl_on_new_line();
			rl_replace_line("", 0);
			rl_redisplay();
			return (WEXITSTATUS(status));
		}
	}
	return (0); // success
}

void	read_heredoc_input(const char *delimiter, int fd, t_cmd *cmd, bool expand_vars)
{
	char	*line;
	char	*processed_line;

	while (42)
	{

		line = readline("> ");
		if (!line)
		{
			ft_putstr_fd("warning: here-document delimited by end-of-file (wanted `", 2);
			ft_putstr_fd((char *)delimiter, 2);
			ft_putstr_fd("')\n", 2);
			break;
		}
		if (ft_strcmp(line, delimiter) == 0 && ft_strlen(line) == ft_strlen(delimiter))
		{
			free(line);
			break;
		}
		if (expand_vars)
		{
			processed_line = process_da_bullshit(line, cmd->shell);
			free(line);
			line = NULL;
		}
		else
			processed_line = line;
		if (processed_line)
		{
			write(fd, processed_line, ft_strlen(processed_line));
			write(fd, "\n", 1);
			free(processed_line);
		}
	}
}

void	handle_heredoc(t_cmd *cmd, char *eof)
{
	char		*temp_name;
	int			exit_code;
	bool		expand_vars;
	char		*clean_eof;

	if (!cmd || !eof)
		return ;
	expand_vars = check_delimiter_quotes(eof);
	clean_eof = remove_quotes(ft_strdup(eof));
	if (!clean_eof)
		return ;
	temp_name = get_random_temp_name();
	if (!temp_name)
	{
		free(clean_eof);
		return ;
	}

	exit_code = handle_heredoc_fork(cmd, clean_eof, temp_name, expand_vars);
	setup_signals();
	if (exit_code == 0)
		create_redir_node(cmd, REDIR_HEREDOC, temp_name);
	safe_free(temp_name);
}
// extern volatile sig_atomic_t	g_signal_received;

// static void	heredoc_signal_handler(int sig)
// {
// 	if (sig == SIGINT)
// 	{
// 		write(STDERR_FILENO, "\n", 1);
// 		g_signal_received = 130;
// 		exit(130);
// 	}
// 	else if (sig == SIGQUIT)
// 	{
// 		/* Explicitly ignore SIGQUIT in heredoc */
// 		signal(SIGQUIT, SIG_IGN);
// 	}
// }

// void	setup_heredoc_signals(void)
// {
// 	signal(SIGINT, heredoc_signal_handler);
// 	signal(SIGQUIT, heredoc_signal_handler);
// }

// static int	open_heredoc_file(const char *temp_name)
// {
// 	int	fd;

// 	fd = open(temp_name, O_CREAT | O_WRONLY | O_TRUNC, 0600);
// 	if (fd == -1)
// 		perror(temp_name);
// 	return (fd);
// }

// static void	handle_heredoc_fork(t_cmd *cmd, char *clean_eof, int fd,
// 				bool expand_vars)
// {
// 	pid_t	pid;

// 	pid = fork();
// 	if (pid == 0)
// 		heredoc_child_process(cmd, clean_eof, fd, expand_vars);
// 	else if (pid > 0)
// 		heredoc_parent_process(pid, cmd, fd, clean_eof);
// 	else
// 	{
// 		close(fd);
// 		free(clean_eof);
// 		perror("fork");
// 		cmd->shell->exit_status = 1;
// 	}
// }

// static void	cleanup_heredoc_resources(int fd, char *filename)
// {
// 	if (fd >= 0)
// 		close(fd);
// 	if (filename)
// 		unlink(filename);
// }

// void	handle_heredoc(t_cmd *cmd, char *eof)
// {
// 	char	*temp_name;
// 	int		fd;
// 	bool	expand_vars;
// 	char	*clean_eof;

// 	if (!cmd || !eof)
// 		return;
// 	expand_vars = check_delimiter_quotes(eof);
// 	clean_eof = remove_quotes(ft_strdup(eof));
// 	if (!clean_eof)
// 		return;
// 	temp_name = get_random_temp_name();
// 	if (!temp_name)
// 	{
// 		free(clean_eof);
// 		return;
// 	}
// 	fd = open_heredoc_file(temp_name);
// 	if (fd == -1)
// 	{
// 		safe_free(clean_eof);
// 		safe_free(temp_name);
// 		return;
// 	}
// 	setup_signals();
// 	handle_heredoc_fork(cmd, clean_eof, fd, expand_vars);
// 	if (g_signal_received != 130)
// 		create_redir_node(cmd, REDIR_HEREDOC, temp_name);
// 	else
// 		cleanup_heredoc_resources(-1, temp_name);
// 	safe_free(temp_name);
// }

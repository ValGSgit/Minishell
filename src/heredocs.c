/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 13:18:31 by vagarcia          #+#    #+#             */
/*   Updated: 2025/05/01 19:50:00 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _GNU_SOURCE
#include "../includes/minishell.h"

extern volatile sig_atomic_t	g_signal_received;

void	heredoc_sigint(int sig)
{
	if (sig == SIGINT)
	{
		g_signal_received = 1;
		ioctl(STDIN_FILENO, TIOCSTI, "\n");
		rl_done = 1;
	}
}

int	handle_heredoc_redirect(t_cmd *cmd, char *clean_eof,
							char *temp_name, bool expand_vars)
{
	int		heredoc_fd;
	void	(*old_sigint)(int);

	old_sigint = signal(SIGINT, heredoc_sigint);
	g_signal_received = 0;
	heredoc_fd = open(temp_name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (heredoc_fd < 0)
	{
		free(clean_eof);
		signal(SIGINT, old_sigint);
		return (1);
	}
	read_heredoc_input(clean_eof, heredoc_fd, cmd, expand_vars);
	close(heredoc_fd);
	free(clean_eof);
	if (g_signal_received)
	{
		cmd->shell->exit_status = 130;
		cmd->syntax_error = 1;
		unlink(temp_name);
		signal(SIGINT, old_sigint);
		return (130);
	}
	signal(SIGINT, old_sigint);
	return (0);
}

static int	handle_child_process(t_cmd *cmd, char *clean_eof,
								char *temp_name, bool expand_vars)
{
	int	heredoc_fd;

	signal(SIGINT, heredoc_sigint);
	signal(SIGQUIT, SIG_IGN);
	heredoc_fd = open(temp_name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (heredoc_fd < 0)
	{
		free(clean_eof);
		cleanup_shell(cmd->shell);
		exit(EXIT_FAILURE);
	}
	g_signal_received = 0;
	read_heredoc_input(clean_eof, heredoc_fd, cmd, expand_vars);
	free(clean_eof);
	if (g_signal_received)
	{
		cleanup_shell(cmd->shell);
		close(heredoc_fd);
		exit(130);
	}
	close(heredoc_fd);
	cleanup_shell(cmd->shell);
	exit(EXIT_SUCCESS);
}

static int	handle_parent_process(t_cmd *cmd, int status)
{
	if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
	{
		cmd->shell->exit_status = 130;
		g_signal_received = SIGINT;
		cleanup_heredocs(cmd->shell);
		cmd->syntax_error = 1;
		return (130);
	}
	else if (WIFSIGNALED(status))
	{
		cmd->shell->exit_status = 130;
		cleanup_heredocs(cmd->shell);
		cmd->syntax_error = 1;
		return (cmd->shell->exit_status);
	}
	return (0);
}

int	handle_heredoc_fork(t_cmd *cmd, char *clean_eof, char *temp_name,
		bool expand_vars)
{
	pid_t	pid;
	int		status;

	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == -1)
	{
		free(clean_eof);
		return (1);
	}
	if (pid == 0)
		handle_child_process(cmd, clean_eof, temp_name, expand_vars);
	else
	{
		free(clean_eof);
		waitpid(pid, &status, 0);
		signal(SIGINT, handle_sigint);
		return (handle_parent_process(cmd, status));
	}
	return (0);
}

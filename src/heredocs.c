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

#include "../includes/minishell.h"

static void	heredoc_signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		write(STDERR_FILENO, "\n", 1);
		exit(130);
	}
	else if (sig == SIGTERM)
	{
		exit(143);
	}
}

void	setup_heredoc_signals(void)
{
	signal(SIGINT, heredoc_signal_handler);
	signal(SIGTERM, heredoc_signal_handler);
}

static int	open_heredoc_file(const char *temp_name)
{
	int	fd;

	fd = open(temp_name, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (fd == -1)
		perror(temp_name);
	return (fd);
}

static void	handle_heredoc_fork(t_cmd *cmd, char *clean_eof, int fd,
				bool expand_vars)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
		heredoc_child_process(cmd, clean_eof, fd, expand_vars);
	else if (pid > 0)
		heredoc_parent_process(pid, cmd, fd, clean_eof);
	else
	{
		close(fd);
		free(clean_eof);
		perror("fork");
		cmd->shell->exit_status = 1;
	}
}

void	handle_heredoc(t_cmd *cmd, char *eof)
{
	char		*temp_name;
	int			fd;
	bool		expand_vars;
	char		*clean_eof;

	if (!cmd || !eof)
		return;
	expand_vars = check_delimiter_quotes(eof);
	clean_eof = remove_quotes(ft_strdup(eof));
	if (!clean_eof)
		return;
	temp_name = get_random_temp_name();
	if (!temp_name)
	{
		free(clean_eof);
		return;
	}
	fd = open_heredoc_file(temp_name);
	if (fd == -1)
	{
		safe_free(clean_eof);
		safe_free(temp_name);
		return;
	}
	handle_heredoc_fork(cmd, clean_eof, fd, expand_vars);
	setup_signals();
	if (g_signal_received != 130)
		create_redir_node(cmd, REDIR_HEREDOC, temp_name);
	safe_free(temp_name);
}

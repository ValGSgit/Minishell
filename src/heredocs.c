/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 13:18:31 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/14 11:27:48 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _GNU_SOURCE
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
	struct sigaction	sa;
	
	sa.sa_handler = heredoc_signal_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGTERM, &sa, NULL);
}

static int	open_heredoc_file(const char *temp_name, char *clean_eof)
{
	int fd;

	fd = open(temp_name, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (fd == -1)
	{
		perror("heredoc");
		free(clean_eof);
	}
	return (fd);
}

static void	handle_heredoc_fork(t_cmd *cmd, char *clean_eof, int fd, bool expand_vars)
{
	pid_t pid;

	pid = fork();
	if (pid == 0)
		heredoc_child_process(cmd, clean_eof, fd, expand_vars);
	else if (pid > 0)
	{
		if (heredoc_parent_process(cmd, fd, clean_eof))
			return ;
	}
	else
	{
		close(fd);
		free(clean_eof);
		perror("fork");
		return ;
	}
}

void	handle_heredoc(t_cmd *cmd, char *eof)
{
	const char *temp_name;
	int fd;
	bool expand_vars;
	char *clean_eof;

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
	fd = open_heredoc_file(temp_name, clean_eof);
	if (fd == -1)
		return ;
	ignore_signals();
	handle_heredoc_fork(cmd, clean_eof, fd, expand_vars);
	setup_signals();
	create_redir_node(cmd, REDIR_HEREDOC, (char *)temp_name);
}

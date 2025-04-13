/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 13:18:31 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/09 12:48:55 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _GNU_SOURCE
#include "../includes/minishell.h"

void cleanup_heredocs(t_shell *shell)
{
	t_redir *redir;
	t_redir *next;

	redir = shell->cmd->redirs;
	while (redir && redir->type == REDIR_HEREDOC)
	{
		if (redir->type == REDIR_HEREDOC && redir->file && unlink(redir->file) == -1)
			perror("unlink");
		free(redir->file);
		next = redir->next;
		free(redir);
		redir = next;
	}
	shell->cmd->redirs = NULL;
}

static const char *get_random_temp_name(void)
{
	static char temp_name[32];
	int fd;
	unsigned char random_bytes[6];
	int i;

	ft_strlcpy(temp_name, HERE_TEMP, sizeof(temp_name));
	fd = open("/dev/urandom", O_RDONLY);
	if (fd == -1 || read(fd, random_bytes, 6) != 6)
	{
		perror("get_random_temp_name");
		if (fd != -1)
			close(fd);
		return (NULL);
	}
	close(fd);
	i = -1;
	while (++i < 6)
		temp_name[13 + i] = '0' + (random_bytes[i] % 10); // Generate digits
	temp_name[19] = '\0';
	return (temp_name);
}

static void heredoc_signal_handler(int sig)
{
    if (sig == SIGINT)
    {
        write(STDERR_FILENO, "\n", 1);
        exit(130); // Bash uses 128 + signal number
    }
    else if (sig == SIGTERM)
    {
        exit(143); // 128 + 15 (SIGTERM)
    }
}

void setup_heredoc_signals(void)
{
    struct sigaction sa;
    
    sa.sa_handler = heredoc_signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    
    // Register signal handlers
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
}

static char *handle_expanded_line(char *arg, char *expanded_line)
{
	if (ft_strcmp(expanded_line, arg) != 0)
	{
		free(arg);
		return (expanded_line);
	}
	return (arg);
}

static char *clean_empty_expansions(char *arg)
{
	char	*result;
	int		i;
	int		j;

	result = malloc(ft_strlen(arg) + 1);
	if (!result)
		return (arg);
	i = 0;
	j = 0;
	while (arg[i])
	{
		if (arg[i] == '$' && (arg[i + 1] == ' ' || arg[i + 1] == '\0' || 
							 arg[i + 1] == '$' || arg[i + 1] == '/'))
			i++;
		else
			result[j++] = arg[i++];
	}
	result[j] = '\0';
	if (j < (int)ft_strlen(arg))
	{
		free(arg);
		return (result);
	}
	free(result);
	return (arg);
}

static char *process_line(char *arg, t_cmd *cmd, bool expand_vars)
{
	char *expanded_line;

	if (!expand_vars)
		return (arg);
	expanded_line = process_argument(arg, cmd->shell);
	if (!expanded_line)
		return (arg);
	arg = handle_expanded_line(arg, expanded_line);
	return (clean_empty_expansions(arg));
}

/**
 * Read heredoc input and write it to the temporary file.
 */
static void read_heredoc_input(const char *delimiter, int fd, t_cmd *cmd, bool expand_vars)
{
	char *line;
	char *processed_line;

	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break;
		}
		
		processed_line = process_line(line, cmd, expand_vars);
		
		if (processed_line)
		{
			write(fd, processed_line, ft_strlen(processed_line));
			write(fd, "\n", 1);
			if (processed_line != line)
				free(processed_line);
		}
	}
}
char	*remove_quotes(char *lim)
{
	char	*new_lim;
	int		i;
	int		j;

	new_lim = malloc(sizeof(char) * (ft_strlen(lim) + 1));
	if (!new_lim)
		return (NULL);
	i = 0;
	j = 0;
	while (lim[i])
	{
		if (lim[i] != '"' && lim[i] != '\'')
			new_lim[j++] = lim[i];
		i++;
	}
	new_lim[j] = '\0';
	free(lim);
	return (new_lim);
}

static void heredoc_child_process(t_cmd *cmd, char *clean_eof, int fd, bool expand_vars)
{
	setup_heredoc_signals();
	read_heredoc_input(clean_eof, fd, cmd, expand_vars);
	close(fd);
	free(clean_eof);
	exit(0);
}

static int heredoc_parent_process(t_cmd *cmd, int fd, char *clean_eof)
{
	int status;

	close(fd);
	free(clean_eof);
	waitpid(-1, &status, 0);
	if (WIFSIGNALED(status))
	{
		cmd->shell->exit_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGINT)
		{
			cmd->shell->signal_status = 1;
			return (1);
		}
	}
	return (0);
}

static bool check_delimiter_quotes(char *eof)
{
	return !((eof[0] == '\'' && eof[ft_strlen(eof) - 1] == '\'') ||
		(eof[0] == '\"' && eof[ft_strlen(eof) - 1] == '\"'));
}

void handle_heredoc(t_cmd *cmd, char *eof)
{
	const char *temp_name;
	int fd;
	pid_t pid;
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
		return;
	}
	fd = open(temp_name, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (fd == -1)
	{
		perror("heredoc");
		free(clean_eof);
		return ;
	}
	ignore_signals();
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
	setup_signals();
	create_redir_node(cmd, REDIR_HEREDOC, (char *)temp_name);
}
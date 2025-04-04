/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 13:18:31 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/04 16:54:19 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	cleanup_heredocs(t_shell *shell)
{
	t_redir	*redir;
	t_redir	*next;

	redir = shell->cmd->redirs;
	while (redir && redir->type == REDIR_HEREDOC)
	{
		if (redir->type == REDIR_HEREDOC && redir->file
			&& unlink(redir->file) == -1)
			perror("unlink");
		free(redir->file);
		next = redir->next;
		free(redir);
		redir = next;
	}
	shell->cmd->redirs = NULL;
}

static const char	*get_random_temp_name(void)
{
	static char		temp_name[32];
	int				fd;
	unsigned char	random_bytes[6];
	int				i;

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
		temp_name[13 + i] = '0' + (random_bytes[i] % 10);
	temp_name[19] = '\0';
	return (temp_name);
}

static char	*process_line(char *arg, t_cmd *cmd)
{
	char	*expanded_line;

	expanded_line = process_argument(arg, cmd->shell);
	if (expanded_line && ft_strcmp(expanded_line, arg) != 0)
	{
		free(arg);
		return (expanded_line);
	}
	return (arg);
}

/**
 * Read heredoc input and write it to the temporary file.
 */
static void	read_heredoc_input(const char *delimiter, int fd, t_cmd *shell)
{
	char	*line;
	char	*expanded_line;

	while (1)
	{
		line = readline("> ");
		if (line && ft_strlen(delimiter) == ft_strlen(line)
			&& ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		expanded_line = process_line(line, shell);
		if (expanded_line)
		{
			write(fd, expanded_line, ft_strlen(expanded_line));
			write(fd, "\n", 1);
			free(expanded_line);
		}
		else
		{
			(write(fd, line, ft_strlen(line)), write(fd, "\n", 1));
			free(line);
		}
	}
}

/**
 * Handle heredoc redirection.
 */
// printf("Temporary file name: %s\n", temp_name);
void	handle_heredoc(t_cmd *cmd, char *eof)
{
	const char	*temp_name;
	int			fd;

	temp_name = get_random_temp_name();
	fd = open(temp_name, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (fd == -1)
	{
		perror("heredoc");
		return ;
	}
	read_heredoc_input(eof, fd, cmd);
	close(fd);
	create_redir_node(cmd, REDIR_HEREDOC, (char *)temp_name);
}

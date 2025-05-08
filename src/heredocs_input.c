/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 19:45:00 by vagarcia          #+#    #+#             */
/*   Updated: 2025/05/08 14:24:57 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	process_input_line(int fd, char *processed_line)
{
	if (processed_line)
	{
		write(fd, processed_line, ft_strlen(processed_line));
		write(fd, "\n", 1);
		free(processed_line);
	}
}

static int	check_delimiter_match(char *line, const char *delimiter)
{
	if (ft_strcmp(line, delimiter) == 0
		&& ft_strlen(line) == ft_strlen(delimiter))
	{
		free(line);
		return (1);
	}
	return (0);
}

static void	handle_eof_warning(const char *delimiter)
{
	ft_putstr_fd("warning: here-document delimited by end-of-file (wanted `",
		2);
	ft_putstr_fd((char *)delimiter, 2);
	ft_putstr_fd("')\n", 2);
}

void	read_heredoc_input(const char *delimiter, int fd, t_cmd *cmd,
						bool expand_vars)
{
	char	*line;
	char	*processed_line;

	while (42)
	{
		if (g_signal_received)
			return ;
		line = readline("> ");
		if (!line)
		{
			if (g_signal_received)
				return ;
			handle_eof_warning(delimiter);
			break ;
		}
		if (check_delimiter_match(line, delimiter))
			break ;
		if (expand_vars)
		{
			processed_line = process_da_bullshit(line, cmd->shell);
		}
		else
			processed_line = line;
		process_input_line(fd, processed_line);
	}
}

void	handle_heredoc(t_cmd *cmd, char *eof)
{
	char	*temp_name;
	int		exit_code;
	bool	expand_vars;
	char	*clean_eof;

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
	exit_code = handle_heredoc_redirect(cmd, clean_eof, temp_name, expand_vars);
	setup_signals();
	if (exit_code == 0)
		create_redir_node(cmd, REDIR_HEREDOC, temp_name);
	else if (exit_code == 130)
		cmd->shell->signal_status = 1;
	if (temp_name)
		free(temp_name);
}

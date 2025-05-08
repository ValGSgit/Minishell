/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   how_many_utils_do_i_need.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 19:52:52 by vagarcia          #+#    #+#             */
/*   Updated: 2025/05/08 14:53:16 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	cleanup_cmd_heredocs(t_cmd *cmd)
{
	t_redir	*redir;
	t_redir	*next;
	t_redir	*prev;

	redir = cmd->redirs;
	prev = NULL;
	while (redir && redir->type == REDIR_HEREDOC)
	{
		next = redir->next;
		if (redir->file)
		{
			if (access(redir->file, F_OK) == 0 && unlink(redir->file) == -1)
				perror("unlink");
		}
		if (redir->prefile)
			free(redir->prefile);
		if (redir->file)
			free(redir->file);
		free(redir);
		redir = next;
	}
	if (prev)
		prev->next = redir;
	else
		cmd->redirs = redir;
}

void	cleanup_heredocs(t_shell *shell)
{
	t_cmd	*cmd;

	if (!shell || !shell->cmd)
		return ;
	cmd = shell->cmd;
	while (cmd)
	{
		cleanup_cmd_heredocs(cmd);
		cmd = cmd->next;
	}
}

int	unclosed_check(char *input)
{
	int		i;
	bool	in_quote;
	char	quote_char;

	i = 0;
	if (!input)
		return (0);
	in_quote = 0;
	while (input[i])
	{
		if (input[i] == '\'' || input[i] == '\"')
		{
			if (!in_quote)
			{
				in_quote = 1;
				quote_char = input[i];
			}
			else if (input[i] == quote_char)
				in_quote = 0;
		}
		i++;
	}
	return (in_quote);
}

void	forked_exit(int code, t_cmd *cmd)
{
	cleanup_shell(cmd->shell);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	exit(code & 255);
}

int	handle_fork_failure(int prev_pipe_in, t_cmd *node, pid_t *pids, \
	int *pipe_fd)
{
	if (prev_pipe_in != 0)
		close(prev_pipe_in);
	if (node->next)
	{
		close(pipe_fd[READ_END]);
		close(pipe_fd[WRITE_END]);
	}
	handle_fork_error(pids);
	return (-1);
}

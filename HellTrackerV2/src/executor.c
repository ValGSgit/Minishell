/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:35:29 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/22 21:30:18 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void setup_input_file(t_cmd *cmd, char *file)
{
    cmd->in_fd = open(file, O_RDONLY);
    if (cmd->in_fd == -1)
    {
        perror("minishell: open");
        cmd->shell->exit_status = 1;
    }
}

static void setup_output_file(t_cmd *cmd, char *file, int append)
{
    if (append)
        cmd->out_fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
    else
        cmd->out_fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (cmd->out_fd == -1)
    {
        perror("minishell: open");
        cmd->shell->exit_status = 1;
    }
}

static void setup_heredoc(t_cmd *cmd, t_redir *redir)
{
    char *temp_name;
    char *clean_eof;
    int fd;
    bool expand_vars;

    expand_vars = check_delimiter_quotes(redir->file);
    clean_eof = ft_strdup(redir->file);
    if (!clean_eof)
        return;
    temp_name = (char *)get_random_temp_name();
    if (!temp_name)
    {
        xfree(clean_eof);  /* Use xfree instead of free */
        return;
    }
    fd = open_heredoc_file(temp_name, clean_eof);
    if (fd == -1)
        return;
    ignore_signals();
    handle_heredoc_fork(cmd, clean_eof, fd, expand_vars);
    setup_signals();
    create_redir_node(cmd, REDIR_HEREDOC, (char *)temp_name);
}

static int	process_heredocs(t_cmd *cmd)
{
    t_redir	*redir;
    char    *temp;

    redir = cmd->redirs;
    while (redir)
    {
        if (redir->type == REDIR_HEREDOC)
        {
            if (redir->file)
            {
                temp = remove_quotes(redir->file);
                xfree(redir->file);
                redir->file = temp;
            }
            else
                redir->file = ft_strdup("");
            if (!redir->file)
                return (-1);
        }
        redir = redir->next;
    }
    return (0);
}

static void	handle_fd_error(t_cmd *cmd)
{
	if (cmd->in_fd == -1 || cmd->out_fd == -1)
	{
		cmd->shell->exit_status = 1;
		return ;
	}
}

static int	setup_redirection(t_cmd *cmd)
{
	t_redir	*redir;

	redir = cmd->redirs;
	cmd->in_fd = STDIN_FILENO;
	cmd->out_fd = STDOUT_FILENO;
	while (redir)
	{
		if (redir->type == REDIR_IN)
			setup_input_file(cmd, redir->file);
		else if (redir->type == REDIR_OUT)
			setup_output_file(cmd, redir->file, 0);
		else if (redir->type == REDIR_APPEND)
			setup_output_file(cmd, redir->file, 1);
		else if (redir->type == REDIR_HEREDOC)
			setup_heredoc(cmd, redir);
		if (cmd->in_fd == -1 || cmd->out_fd == -1)
			return (-1);
		redir = redir->next;
	}
	return (0);
}

static int	execute_single_builtin(t_cmd *cmd)
{
	int	stdin_fd;
	int	stdout_fd;
	int	result;

	stdin_fd = dup(STDIN_FILENO);
	stdout_fd = dup(STDOUT_FILENO);
	if (cmd->in_fd != STDIN_FILENO)
		dup2(cmd->in_fd, STDIN_FILENO);
	if (cmd->out_fd != STDOUT_FILENO)
		dup2(cmd->out_fd, STDOUT_FILENO);
	execute_builtin(cmd);
	result = cmd->shell->exit_status;
	dup2(stdin_fd, STDIN_FILENO);
	dup2(stdout_fd, STDOUT_FILENO);
	close(stdin_fd);
	close(stdout_fd);
	if (cmd->in_fd != STDIN_FILENO)
		close(cmd->in_fd);
	if (cmd->out_fd != STDOUT_FILENO)
		close(cmd->out_fd);
	return (result);
}

void	execute_command(t_cmd *cmd)
{
	if (!cmd)
		return ;
	if (process_heredocs(cmd) != 0)
		return ;
	if (setup_redirection(cmd) == -1)
	{
		handle_fd_error(cmd);
		return ;
	}
	if (!cmd->args)
		return ;
	if (!cmd->next && is_builtin(cmd->args[0]))
	{
		cmd->shell->exit_status = execute_single_builtin(cmd);
		return ;
	}
	execute_pipeline(cmd, cmd->shell);
}
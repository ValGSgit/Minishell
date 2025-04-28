#include "../includes/minishell.h"


void	apply_redirection(t_cmd *cmd, bool fork)
{
	t_redir *redir_node;

	redir_node = cmd->redirs;
	if (cmd->redirs->file && cmd->redirs->file[0] == '\0')
	{
		cmd->shell->exit_status = 1;
		ft_putstr_fd("Minishell: ", 2);
		ft_putstr_fd(cmd->redirs->prefile, 2);
		ft_putstr_fd(": ambiguous redirect\n", 2);
		if(fork)
			exit(1);
		return;
	} 
	while (redir_node)
	{
		if (redir_node->type == REDIR_IN)
			handle_redirection_in(cmd);
		else if (redir_node->type == REDIR_OUT)
			handle_redirection_out(0, cmd);
		else if (redir_node->type == REDIR_APPEND)
			handle_redirection_out(1, cmd);
		else if (redir_node->type == REDIR_HEREDOC)
			handle_redirection_in(cmd);
		redir_node = redir_node->next;
	}
}

void	handle_redirection_in(t_cmd *cmd)
{
	int fd;

	fd = open(cmd->redirs->file, O_RDONLY);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->redirs->file, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		cmd->shell->exit_status = 1;
		exit(1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
}

void	handle_redirection_out(int append, t_cmd *cmd)
{
	int fd;

	if (append)
		fd = open(cmd->redirs->file, O_WRONLY | O_APPEND | O_CREAT, 0644);
	else
		fd = open(cmd->redirs->file, O_WRONLY | O_CREAT | O_TRUNC, 0644); 
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->redirs->file, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		cmd->shell->exit_status = 1;
		exit(1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
}


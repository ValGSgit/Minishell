#include "../includes/minishell.h"


void	apply_redirection(t_cmd *cmd)
{
	t_redir *redir;

	redir = cmd->redirs;
	while (redir)
	{
		if (redir->type == REDIR_IN)
			handle_redirection_in(redir, cmd);
		else if (redir->type == REDIR_OUT)
			handle_redirection_out(redir, 0, cmd);
		else if (redir->type == REDIR_APPEND)
			handle_redirection_out(redir, 1, cmd);
		else if (redir->type == REDIR_HEREDOC)
			handle_redirection_in(redir, cmd);
		redir = redir->next;
	}
}

void	handle_redirection_in(t_redir *redir, t_cmd *cmd)
{
	int fd;

	fd = open(redir->file, O_RDONLY);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(redir->file, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		cmd->shell->exit_status = 1;
		exit(1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
}

void	handle_redirection_out(t_redir *redir, int append, t_cmd *cmd)
{
	int fd;

	if (append)
		fd = open(redir->file, O_WRONLY | O_APPEND | O_CREAT, 0644);
	else
		fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644); 
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(redir->file, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		cmd->shell->exit_status = 1;
		exit(1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
}

/* only_redir_fork function removed - using version from exec_fork_utils.c */
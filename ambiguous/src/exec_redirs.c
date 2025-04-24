#include "../includes/minishell.h"


void	apply_redirection(t_cmd *cmd)
{
	t_redir *redir;

	redir = cmd->redirs;
	if (cmd->redirs->file[0] == '\0')
	{
		cmd->shell->exit_status = 1;
		ft_putstr_fd("Minishell: ", 2);
		ft_putstr_fd(cmd->redirs->prefile, 2);
		ft_putstr_fd(": ambiguous redirect\n", 2);
		exit(1);
	}
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
	exit(0);
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

/* int	handle_only_redirection(t_cmd *cmd)
{
	if (!cmd->args && cmd->redirs)
	{
		only_redir_fork(cmd);
		return (1);
	}
	return (0);
} */


void	handle_fork_error(pid_t *pids)
{
	perror("minishell: fork");
	free(pids);
}
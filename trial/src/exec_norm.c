/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_norm.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 14:59:22 by vagarcia          #+#    #+#             */
/*   Updated: 2025/05/03 17:26:06 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	executor(t_cmd *cmd, t_shell *shell)
{
	t_cmd	*node;

	node = cmd;
	if (!node)
	{
		shell->exit_status = 0;
		return ;
	}
	if (!cmd->next)
		execute_single_command(cmd, shell);
	else
		execute_pipeline(node, shell);
}

void	execute_forked_command(t_cmd *cmd, t_shell *shell)
{
	if (cmd->redirs)
		apply_redirection(cmd, true);
	if (cmd->args && cmd->args[0] && is_builtin(cmd->args[0]))
	{
		execute_builtin(cmd);
		forked_exit(cmd->shell->exit_status, cmd);
	}
	else if (cmd->args && cmd->args[0])
		execute_external_command(cmd, shell);
	forked_exit(cmd->shell->exit_status & 255, cmd);
}

/* void	execute_single_command(t_cmd *cmd, t_shell *shell)
{
	pid_t	pid;

	if (cmd->args && cmd->args[0] && cmd->args[0][0] == '\0'
		&& !cmd->args[1] && !cmd->redirs)
		return ;
		
	if (cmd->args && cmd->args[0] && is_builtin(cmd->args[0]))
	{
		if (cmd->redirs)
		{
			cmd->in_fd = dup(0);
			cmd->out_fd = dup(1);
			apply_redirection(cmd, false);
			execute_builtin(cmd);
			dup2(cmd->in_fd, 0);
			dup2(cmd->out_fd, 1);
			close(cmd->in_fd);
			close(cmd->out_fd);
		}
		execute_builtin(cmd);
		return ;
	}
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == 0)
		execute_forked_command(cmd, shell);
	waitpid_for_single_command(pid, shell);
	signal(SIGINT, handle_sigint);
} */
void	execute_single_command(t_cmd *cmd, t_shell *shell)
{
	pid_t	pid;

	if (cmd->args && cmd->args[0] && cmd->args[0][0] == '\0'
		&& !cmd->args[1] && !cmd->redirs)
		return ;
		
	if ((cmd->args && cmd->args[0] && is_builtin(cmd->args[0])) || (cmd->args && cmd->args[0] && is_env_cmd(cmd->args[0]) && cmd->redirs))
	{
		if (cmd->redirs)
		{
			cmd->in_fd = dup(0);
			cmd->out_fd = dup(1);
			apply_redirection(cmd, false);
			execute_builtin(cmd);
			dup2(cmd->in_fd, 0);
			dup2(cmd->out_fd, 1);
			close(cmd->in_fd);
			close(cmd->out_fd);
			return ;
		}
		execute_builtin(cmd);
		return ;
	}
	if (cmd->redirs)
	{
		pid = fork();
		if (pid == 0)
			execute_forked_command(cmd, shell);
		waitpid_for_single_command(pid, shell);
		return ;
	}
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == 0)
		execute_forked_command(cmd, shell);
	waitpid_for_single_command(pid, shell);
	signal(SIGINT, handle_sigint);
}
void	execute_builtin_statement(t_cmd *cmd)
{
	if (is_env_cmd(cmd->args[0]) || is_builtin(cmd->args[0]))
		execute_builtin(cmd);
	return ;
}

char	*get_clean_cmd_name(const char *path)
{
	const char	*last_slash;

	if (!path)
		return (NULL);
	last_slash = ft_strrchr(path, '/');
	if (last_slash)
		return ((char *)(last_slash + 1));
	return ((char *)path);
}

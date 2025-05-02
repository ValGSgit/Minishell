/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_norm.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 14:59:22 by vagarcia          #+#    #+#             */
/*   Updated: 2025/05/02 17:34:08 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	executor(t_cmd *cmd, t_shell *shell)
{
	if (!cmd)
	{
		shell->exit_status = 0;
		return ;
	}
	if (!cmd->next)
		execute_single_command(cmd, shell);
	else
		execute_pipeline(cmd, shell);
}

void	execute_forked_command(t_cmd *cmd, t_shell *shell)
{
	if (cmd->redirs)
		apply_redirection(cmd, true);
	if (cmd->args && cmd->args[0] && is_builtin(cmd->args[0]))
	{
		execute_builtin(cmd);
		close_cmd_fds(cmd);
		cleanup_shell(cmd->shell);
		exit(shell->exit_status);
	}
	else if (cmd->args && cmd->args[0])
		execute_external_command(cmd, shell);
	close_cmd_fds(cmd);
	cleanup_shell(cmd->shell);
	exit(shell->exit_status % 256);
}

void	execute_single_command(t_cmd *cmd, t_shell *shell)
{
	pid_t	pid;

	if (cmd->args && cmd->args[0] && cmd->args[0][0] == '\0'
		&& !cmd->args[1] && !cmd->redirs)
		return ;
	if (cmd->redirs)
	{
		pid = fork();
		if (pid == 0)
			execute_forked_command(cmd, shell);
		waitpid_for_single_command(pid, shell);
		reset_redirections(cmd); // Reset file descriptors after command execution
		return ;
	}
	if (cmd->args && cmd->args[0] && is_builtin(cmd->args[0]))
	{
		execute_builtin(cmd);
		reset_redirections(cmd); // Reset file descriptors after builtin execution
		return ;
	}
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == 0)
		execute_forked_command(cmd, shell);
	waitpid_for_single_command(pid, shell);
	reset_redirections(cmd); // Reset file descriptors after command execution
	signal(SIGINT, handle_sigint);
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

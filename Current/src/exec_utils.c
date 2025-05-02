/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 15:45:00 by vagarcia          #+#    #+#             */
/*   Updated: 2025/05/01 12:57:16 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	waitpid_for_single_command(pid_t pid, t_shell *shell)
{
	int	status;

	(void)shell;
	status = 0;
	waitpid(pid, &status, 0);
}

void	execute_builtin_or_exit(t_cmd *cmd)
{
	execute_builtin(cmd);
	exit(cmd->shell->exit_status & 0xFF);
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

void	execve_error(t_cmd *cmd)
{
	char	*cmd_name;

	if (!cmd || !cmd->args || !cmd->args[0])
		return ;
	cmd_name = get_clean_cmd_name(cmd->args[0]);
	print_error_message(cmd_name, ": ", strerror(errno));
	ft_putstr_fd("\n", 2);
	exit(126);
}

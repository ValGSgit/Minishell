/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_extra1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iionescu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 15:40:13 by iionescu          #+#    #+#             */
/*   Updated: 2025/05/02 15:40:15 by iionescu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	wait_for_pipeline(pid_t *pids, int count, pid_t last_pid,
	t_shell *shell)
{
	int	i;
	int	status;

	i = 0;
	status = 0;
	while (i < count)
	{
		if (pids[i] == last_pid)
		{
			waitpid(pids[i], &status, 0);
			if (WIFEXITED(status))
				shell->exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
			{
				shell->exit_status = 128 + WTERMSIG(status);
				if (WTERMSIG(status) == SIGINT)
					write(STDERR_FILENO, "\n", 1);
			}
		}
		else
			waitpid(pids[i], NULL, 0);
		i++;
	}
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_DFL);
}

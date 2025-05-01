/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_plus.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iionescu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 15:45:15 by iionescu          #+#    #+#             */
/*   Updated: 2025/05/01 15:45:22 by iionescu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_shell(t_shell *shell)
{
	if (!shell)
		return ;
	if (shell->env)
		free_env(shell->env);
	if (shell->cmd)
		free_cmd(shell->cmd);
}
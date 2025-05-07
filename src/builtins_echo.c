/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_echo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 14:15:00 by vagarcia          #+#    #+#             */
/*   Updated: 2025/05/05 13:17:53 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	process_options(t_cmd *cmd, int *i)
{
	int	j;
	int	n_flag;

	n_flag = 0;
	while (cmd->args[*i] && cmd->args[*i][0] == '-')
	{
		j = 1;
		while (cmd->args[*i][j] && cmd->args[*i][j] == 'n')
			j++;
		if (cmd->args[*i][j] != '\0' || j == 1)
			break ;
		n_flag = 1;
		(*i)++;
	}
	return (n_flag);
}

void	ft_echo(t_cmd *cmd)
{
	int	i;
	int	n_flag;

	i = 1;
	if (!cmd->args[1])
	{
		ft_putchar_fd('\n', STDOUT_FILENO);
		cmd->shell->exit_status = 0;
		return ;
	}
	n_flag = process_options(cmd, &i);
	while (cmd->args[i])
	{
		ft_putstr_fd(cmd->args[i], STDOUT_FILENO);
		if (cmd->args[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
	if (!n_flag)
		ft_putchar_fd('\n', STDOUT_FILENO);
	cmd->shell->exit_status = 0;
}

int	ft_isnumber(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

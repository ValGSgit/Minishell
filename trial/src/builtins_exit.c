/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_exit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 18:52:16 by vagarcia          #+#    #+#             */
/*   Updated: 2025/05/03 15:50:23 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	handle_exit_error(char *arg, t_cmd *cmd)
{
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
	forked_exit(2, cmd);
}

static int	is_invalid_number(const char *str)
{
	int	i;
	int	has_digits;

	i = 0;
	has_digits = 0;
	while (str[i] && (str[i] == ' ' || str[i] == '\t' || str[i] == '\n'
			|| str[i] == '\v' || str[i] == '\f' || str[i] == '\r'))
		i++;
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		has_digits = 1;
		i++;
	}
	while (str[i] && (str[i] == ' ' || str[i] == '\t' || str[i] == '\n'
			|| str[i] == '\v' || str[i] == '\f' || str[i] == '\r'))
		i++;
	if (!str[i] && has_digits)
		return (0);
	return (1);
}

static long long	recursive_atol(const char *str, int i, int sign,
		long long result)
{
	if (!str[i] || str[i] < '0' || str[i] > '9')
		return (result * sign);
	if (sign == -1 && result == 922337203685477580LL && str[i] == '8')
		return (LLONG_MIN);
	if ((result > LLONG_MAX / 10) || (result == LLONG_MAX / 10 && (str[i]
				- '0') > LLONG_MAX % 10))
	{
		errno = ERANGE;
		return (0);
	}
	return (recursive_atol(str, i + 1, sign, result * 10 + (str[i] - '0')));
}

static long long	ft_atol_improved(const char *str)
{
	int	i;
	int	sign;

	i = 0;
	sign = 1;
	errno = 0;
	while (str[i] && (str[i] == ' ' || str[i] == '\t' || str[i] == '\n'
			|| str[i] == '\v' || str[i] == '\f' || str[i] == '\r'))
		i++;
	while (str[i] && str[i] == '0')
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] && str[i] == '0')
		i++;
	return (recursive_atol(str, i, sign, 0));
}

void	ft_exit(t_cmd *cmd)
{
	long long	exit_code;
	char		*cmd_arg;

	if (cmd->shell->is_interactive && !cmd->next)
		ft_putstr_fd("exit\n", 2);
	exit_code = cmd->shell->exit_status;
	if (!cmd->args[1])
	{
		forked_exit(exit_code & 0xFF, cmd);
	}
	cmd_arg = cmd->args[1];
	if (is_invalid_number(cmd_arg))
		handle_exit_error(cmd->args[1], cmd);
	exit_code = ft_atol_improved(cmd_arg);
	if (errno == ERANGE)
		handle_exit_error(cmd->args[1], cmd);
	if (cmd->args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		cmd->shell->exit_status = 1;
		return ;
	}
	forked_exit(exit_code & 0xFF, cmd);
}

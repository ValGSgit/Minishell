/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_exit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 18:52:16 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/30 20:58:41 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * Handles exit error by printing message and exiting with code 2
 */
static void	handle_exit_error(char *arg)
{
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
	exit(2);
}

/**
 * Checks if string is a valid number
 * Returns 1 if string is invalid, 0 if valid
 */
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

/**
 * Custom implementation of atol
 * Doesn't use loops, recursive approach instead
 */
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

/**
 * Converts string to long long
 */
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

/**
 * Built-in exit command implementation
 */
void	ft_exit(t_cmd *cmd)
{
	long long	exit_code;
	char		*cmd_arg;

	if (cmd->shell->is_interactive && !cmd->next)
		ft_putstr_fd("exit\n", 2);
	exit_code = cmd->shell->exit_status;
	if (!cmd->args[1])
	{
		cleanup_shell(cmd->shell);
		exit(exit_code & 0xFF);
	}
	cmd_arg = cmd->args[1];
	if (is_invalid_number(cmd_arg))
		handle_exit_error(cmd->args[1]);
	exit_code = ft_atol_improved(cmd_arg);
	if (errno == ERANGE)
		handle_exit_error(cmd->args[1]);
	if (cmd->args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		cmd->shell->exit_status = 1;
		return ;
	}
	cleanup_shell(cmd->shell);
	exit(exit_code & 0xFF);
}

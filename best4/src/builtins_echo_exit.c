/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_echo_exit.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 14:15:00 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/28 13:07:59 by vagarcia         ###   ########.fr       */
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
		while (ft_isspace(cmd->args[*i][j]))
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

static void	handle_exit_error(char *arg)
{
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(": numeric argument required\n", 2);
	exit(2);
}

static long handle_overflow(int sign, char **endptr, const char *str)
{
    errno = ERANGE;
    if (endptr)
        *endptr = (char *)str;
    if (sign == -1)
        return LONG_MIN;
    else
        return LONG_MAX;
}

static long ft_atli(const char *str, char **endptr)
{
    long result = 0;
    int sign = 1;
    unsigned long cutoff;
    while (*str == ' ' || (*str >= '\t' && *str <= '\r'))
        str++;
    if (*str == '-')
    {
        sign = -1;
        str++;
    }
    else if (*str == '+')
        str++;
    if (sign == -1)
        cutoff = -(unsigned long)LONG_MIN;
    else
        cutoff = LONG_MAX;
    while (*str >= '0' && *str <= '9')
    {
        int digit = *str - '0';
        if (result > cutoff / 10 || (result == cutoff / 10 && digit > cutoff % 10))
            return handle_overflow(sign, endptr, str);
        result = result * 10 + digit;
        str++;
    }
    if (endptr)
        *endptr = (char *)str;
    return result * sign;
}

void	ft_exit(t_cmd *cmd)
{
	int			exit_code;
	long long	num;
	char		*endptr;

	if (cmd->shell->is_interactive && !cmd->next)
		ft_putstr_fd("exit\n", 2);
	if (!cmd->args[1])
	{
		exit(cmd->shell->exit_status);
	}
	errno = 0;
	num = ft_atli(cmd->args[1], &endptr, 10);
	if (*endptr != '\0' || errno == ERANGE)
	{
		handle_exit_error(cmd->args[1]);
	}
	exit_code = (unsigned char)num;
	if (cmd->args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		cmd->shell->exit_status = 1;
		return ;
	}
	exit(exit_code);
}

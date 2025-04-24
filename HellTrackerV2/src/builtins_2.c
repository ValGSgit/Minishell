/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:27:38 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/22 15:27:38 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

static void	exit_with_cleanup(int exit_code, t_cmd *cmd)
{
	t_shell *shell;

	/* Store shell pointer before freeing cmd */
	shell = cmd->shell;
	
	/* Clear readline history */
	rl_clear_history();
	
	/* Clean up environment if needed */
	if (shell && shell->env) 
	{
		/* Mark environment variables as manually freed to avoid double free */
		int i = 0;
		while (shell->env[i])
		{
			mark_no_free(shell->env[i]);
			i++;
		}
		mark_no_free(shell->env);
		free_env(shell->env);
		shell->env = NULL;
	}
	
	/* Clean any temporary files used by the shell */
	cleanup_temp_files();
	
	/* Clean any remaining memory without trying to free already-freed pointers */
	clean_memory();
	
	/* Exit with provided code */
	exit(exit_code);
}

static void	exit_with_error(char *arg, t_cmd *cmd)
{
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(": numeric argument required\n", 2);
	exit_with_cleanup(2, cmd);
}

void	ft_exit(t_cmd *cmd)
{
	int			exit_code;
	long long	num;
	char		*endptr;

	if (cmd->shell->is_interactive && !cmd->next)
		ft_putstr_fd("exit\n", 2);
	if (!cmd->args[1])
		exit_with_cleanup(cmd->shell->exit_status, cmd);
	errno = 0;
	num = strtol(cmd->args[1], &endptr, 10);
	if (*endptr != '\0' || errno == ERANGE)
		exit_with_error(cmd->args[1], cmd);
	exit_code = (unsigned char)num;
	if (cmd->args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		cmd->shell->exit_status = 1;
		return ;
	}
	exit_with_cleanup(exit_code, cmd);
}

static void	shift_env_variables(t_cmd *cmd, int i)
{
	int	j;

	free(cmd->shell->env[i]);
	j = i;
	while (cmd->shell->env[j + 1])
	{
		cmd->shell->env[j] = cmd->shell->env[j + 1];
		j++;
	}
	cmd->shell->env[j] = NULL;
}

static void	remove_env_var(t_cmd *cmd, char *arg)
{
	int		i;
	size_t	arg_len;
	bool	is_path;

	i = 0;
	arg_len = ft_strlen(arg);
	is_path = (ft_strcmp(arg, "PATH") == 0);
	if (!cmd->shell->env)
		return ;
	while (cmd->shell->env[i])
	{
		if ((ft_strncmp(cmd->shell->env[i], arg, arg_len) == 0
			&& (cmd->shell->env[i][arg_len] == '='
			|| cmd->shell->env[i][arg_len] == '\0')))
		{
			shift_env_variables(cmd, i);
			if (is_path)
				cmd->shell->path_unset = true;
			return ;
		}
		i++;
	}
}

static void	handle_path_unset(t_cmd *cmd, char *arg)
{
	char	*path_value;

	if (ft_strcmp(arg, "PATH") == 0)
	{
		path_value = get_env_value("PATH", cmd->shell->env);
		if (path_value)
			cmd->shell->path_unset = true;
	}
	remove_env_var(cmd, arg);
	if (ft_strcmp(arg, "PATH") == 0)
	{
		if (get_env_value("PATH", cmd->shell->env))
			cmd->shell->path_unset = false;
	}
}

void	ft_unset(t_cmd *cmd)
{
	int	i;
	int	had_error;

	had_error = 0;
	if (!cmd->args[1])
	{
		cmd->shell->exit_status = 0;
		return ;
	}
	i = 1;
	while (cmd->args[i])
	{
		if (cmd->shell->env)
		{
			handle_path_unset(cmd, cmd->args[i]);
		}
		i++;
	}
	if (had_error)
		cmd->shell->exit_status = 1;
	else
		cmd->shell->exit_status = 0;
}
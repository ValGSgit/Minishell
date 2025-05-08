/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_extra1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 18:57:06 by vagarcia          #+#    #+#             */
/*   Updated: 2025/05/08 16:25:48 by vagarcia         ###   ########.fr       */
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

void	initialize_shell_env(t_shell *shell, char *prog_name)
{
	char	*cwd;
	char	*pwd_var;

	if (ft_strcmp(prog_name, "minishell") == 0 || ft_strcmp(prog_name,
			"./minishell") == 0 || ft_strcmp(prog_name, "bash") == 0)
		update_shlvl(shell);
	if (!get_env_value("PWD", shell->env))
	{
		cwd = getcwd(NULL, 0);
		if (cwd)
		{
			pwd_var = safe_strjoin("PWD=", cwd, 0);
			if (pwd_var)
				update_or_add_env(pwd_var, &shell->env);
			safe_free(pwd_var);
			safe_free(cwd);
		}
	}
}

void	initialize_shell(t_shell *shell, char **argv)
{
	char	*prog_name;

	prog_name = ft_strrchr(argv[0], '/');
	if (prog_name)
		prog_name++;
	else
		prog_name = argv[0];
	initialize_shell_env(shell, prog_name);
}

char	*calculate_logical_path(const char *current_pwd, const char *path)
{
	char	**path_parts;
	char	*full_path;
	char	*result;
	int		count;

	if (!current_pwd || !path)
		return (NULL);
	full_path = build_full_path(current_pwd, path);
	if (!full_path)
		return (NULL);
	path_parts = ft_split(full_path, '/');
	free(full_path);
	if (!path_parts)
		return (NULL);
	count = 0;
	while (path_parts[count])
		count++;
	result = resolve_path_parts(path_parts, count);
	free_tokens(path_parts);
	return (result);
}

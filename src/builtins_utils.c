/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 10:33:07 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/04 16:55:21 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


void	update_pwd(t_shell *shell)
{
	char	*cwd;
	char	*new_pwd;
	int		i;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return ;
	new_pwd = ft_strjoin("PWD=", cwd);
	free(cwd);
	i = 0;
	while (shell->env[i])
	{
		if (ft_strncmp(shell->env[i], "PWD=", 4) == 0)
		{
			free(shell->env[i]);
			shell->env[i] = new_pwd;
			return ;
		}
		i++;
	}
	update_or_add_env(new_pwd, &shell->env);
	free(new_pwd);
}

void	ft_cd(t_cmd *cmd)
{
	char	*path;

	if (!cmd->args[1])
		path = getenv("HOME");
	if (cmd->args[2])
	{
		write(2, "cd: too many arguments\n", 24);
		cmd->exit_status = 1;
		exit(1);
	}
	else
		path = cmd->args[1];
	if (chdir(path) == -1)
	{
		perror("cd");
		cmd->shell->exit_status = 1;
	}
	else
		cmd->shell->exit_status = 0;
	update_pwd(cmd->shell);
}

void	ft_pwd(t_cmd *cmd)
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
		printf("%s\n", cwd);
	else
		perror("pwd");
	cmd->exit_status = 0;
}

void ft_env(t_cmd *cmd)
{
    int i;

    if (!cmd->shell->env)
        return;
    if (cmd->args[1])
    {
        cmd->exit_status = 1;
        return;
    }
	i = 0;
    while (cmd->shell->env[i])
    	printf("%s\n", cmd->shell->env[i++]);
}
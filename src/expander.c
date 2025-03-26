/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:57:26 by vagarcia          #+#    #+#             */
/*   Updated: 2025/03/24 12:52:51 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/planer.h"

char	*get_env_value(char *name, char **env)
{
	int		i;
	char	*key;

	i = 0;
	if (!env || !name)
		return (NULL);
	while (env[i])
	{
		key = ft_substr(env[i], 0, (char *)ft_strchr(env[i], '=') - env[i]);
		if (ft_strncmp(key, name, ft_strlen(key)) == 0)
		{
			free(key);
			return (ft_strchr(env[i], '=') + 1);
		}
		free(key);
		i++;
	}
	return (NULL);
}

char	*resolve_path(char *cmd, char **env)
{
	int		i;
	char	*full_path;
	char	*temp;
	char	**paths;

	i = 0;
	paths = ft_split(get_env_value("PATH", env), ':');
	if (!paths)
		return (cmd);
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(temp, cmd);
		free(temp);
		if (access(full_path, F_OK | X_OK) == 0)
		{
			free_env(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_env(paths);
	return (cmd);
}

static char	*expand_variable(char *arg, t_shell *shell)
{
    char	*value;
    char	*result;

    if (ft_strcmp(arg, "$?") == 0) // Handle exit status
        return (ft_itoa(shell->exit_status));
    if (ft_strcmp(arg, "$-") == 0) // Handle shell flags
        return (ft_strdup("himBHs"));
    if (arg[1] == '\0') // Handle single '$'
        return (ft_strdup("$"));
    if (arg[0] == '$' && ft_isdigit(arg[1])) // Handle $ followed by digits
        return (ft_strdup("")); // Positional parameters not supported
    if (arg[0] == '$' && !ft_isspace(arg[1]))
    {
        value = get_env_value(arg + 1, shell->env); // Get value from environment
        if (value)
        {
            result = ft_strjoin(value, arg); // Append remaining string
            return (result);
        }
        return (ft_strdup(arg + 1)); // Return remaining string if no match
    }
    return (ft_strdup(arg)); // Default case
}

// void	expander(t_cmd *cmd, t_shell *shell)
// {
// 	int		i;
// 	char	*value;

// 	i = 0;
// 	if (!cmd->args)
// 		return ;
// 	if (ft_strncmp(cmd->args[0], "./", 2) != 0 && ft_strncmp(cmd->args[0],
// 			"../", 3) != 0)
// 		cmd->args[0] = resolve_path(cmd->args[0], shell->env);
// 	while (cmd->args[++i])
// 	{
// 		if (ft_strchr(cmd->args[i], '$'))
// 		{
// 			value = expand_variable(cmd->args[i], shell);
// 			if (value)
// 			{
// 				free(cmd->args[i]);
// 				cmd->args[i] = value;
// 			}
// 		}
// 	}
// }

void	expander(t_cmd *cmd, t_shell *shell)
{
    int		i;
    char	*value;

    i = 0;
    if (!cmd->args)
        return ;
    if (ft_strncmp(cmd->args[0], "./", 2) != 0 && ft_strncmp(cmd->args[0],
            "../", 3) != 0)
        cmd->args[0] = resolve_path(cmd->args[0], shell->env);
    while (cmd->args[++i])
    {
        if (ft_strchr(cmd->args[i], '$'))
        {
            value = expand_variable(cmd->args[i], shell);
            if (value)
            {
                free(cmd->args[i]);
                cmd->args[i] = value;
            }
        }
    }
}
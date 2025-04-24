/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:57:26 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/14 12:20:38 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*append_str(char *dest, char *src)
{
	char	*new_str;

	if (!dest)
	{
		if (src)
			return (ft_strdup(src));
		else
			return (ft_strdup(""));
	}
	if (src && *src)
		new_str = ft_strjoin(dest, src);
	else
		new_str = ft_strjoin(dest, "");
	free(dest);
	return (new_str);
}

/**
 * Processes a single argument, expanding any variables as needed
 * Returns the expanded string
 */
char	*process_argument(char *arg, t_shell *shell)
{
	t_expander_state	state;
	int					i;
	char				*value;

	ft_memset(&state, 0, sizeof(t_expander_state));
	state.result = ft_strdup("");
	if (!state.result || !arg)
		return (state.result);
	i = -1;
	while (arg[++i])
	{
		if (arg[i] == '\'' && !state.in_dquote)
			state.in_quote = !state.in_quote;
		else if (arg[i] == '"' && !state.in_quote)
			state.in_dquote = !state.in_dquote;
		else if (arg[i] == '$' && !state.in_quote)
		{
			value = expand_variable(arg, &i, shell);
			if ((value && *value))
				state.result = append_str(state.result, value);
			free(value);
		}
		else
			state.result = append_str(state.result, (char[]){arg[i], '\0'});
	}
	return (state.result);
}

static char	*resolve_path_in_current_dir(char *cmd)
{
	char	*path;
	char 	*temp;
	char	*current_dir;

	current_dir = getcwd(NULL, 0);
	if (!current_dir)
		return (NULL);
	temp = ft_strjoin(current_dir, "/");
	free(current_dir);
	if (!temp)
		return (NULL);
	path = ft_strjoin(temp, cmd);
	free(temp); // fixed unset PATH leak
	return (path);
}

void	expander(t_cmd *cmd, t_shell *shell)
{
	char	*expanded;

	if (!cmd || !cmd->args)
		return ;
	if (cmd->args[0] && ft_strcmp(cmd->args[0], ".") == 0 && !cmd->args[1])
	{
		ft_putstr_fd("Minishell: .: filename argument required\n", 2);
		ft_putstr_fd(".: usage: . filename [arguments]\n", 2);
		shell->exit_status = 2;
		cmd->args = NULL; // so it doesnt go into execution
		cmd->syntax_error = 1;
		return ;
	}
	if (cmd->args[0] && ft_strcmp(cmd->args[0], "..") == 0 && !cmd->args[1])
	{
		ft_putstr_fd("..: command not found\n", 2);
		shell->exit_status = 127;
		cmd->args = NULL; // so it doesnt go into execution
		cmd->syntax_error = 1;
		return ;
	}
	if (cmd->args[0] && ft_strchr(cmd->args[0], '/') == NULL
		&& !is_builtin(cmd->args[0]))
	{
		if (shell->env && get_env_value("PATH", shell->env))
			expanded = resolve_path(cmd->args[0], shell->env);
		else
			expanded = resolve_path_in_current_dir(cmd->args[0]);
		// if (cmd->args[0])
		//    free(cmd->args[0]);
		free(cmd->args[0]); //fixed? arg[0] leak was strdup 2 times
		cmd->args[0] = expanded;
	}
/*  	int j = 0;
	while(cmd->args[j])
	{
		free(cmd->args[j]);
		j++;
	}
	free(cmd->args);  */
	cmd->args = apply_word_splitting(cmd->args, shell);
}

/* Remove null or empty strings from args array */
// static char	**ft_clean_args(char **args)
// {
// 	int		i;
// 	int		j;
// 	char	**cleaned;

// 	i = 0;
// 	j = 0;
// 	if (!args)
// 		return (NULL);
// 	while (args[i])
// 		i++;
// 	cleaned = malloc(sizeof(char *) * (i + 1));
// 	i = 0;
// 	if (!cleaned)
// 		return (NULL);
// 	while (args[i])
// 	{
// 		if (args[i] && *args[i])
// 			cleaned[j++] = args[i];
// 		else
// 			free(args[i]);
// 		i++;
// 	}
// 	cleaned[j] = NULL;
// 	free(args);
// 	return (cleaned);
// }

void	expand_nodes(t_cmd *cmd, t_shell *shell)
{
	t_cmd	*node;
    char    *reresolved;

	node = cmd;
	shell->cmd = node;
	while (node)
	{
		expander(node, shell);
       // expander(node, shell);
		node->shell = shell;
		//if (node->args)
        //    node->args = ft_clean_args(node->args);
        if (node->args && node->args[0] && ft_strchr(node->args[0], '/') == NULL
            && !is_builtin(node->args[0]))
        {
            if (shell->env && get_env_value("PATH", shell->env))
		    	reresolved = resolve_path(cmd->args[0], shell->env);
		    else
		    	reresolved = resolve_path_in_current_dir(cmd->args[0]);
            if (cmd->args[0])
                free(cmd->args[0]);
		    cmd->args[0] = reresolved;
        }
        node = node->next;
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs_more_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 17:16:14 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/22 22:50:42 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// static int	should_expand_variable(char *line, int i)
// {
// 	if (line[i] == '$' && line[i + 1] && line[i + 1] != ' ' && 
// 		line[i + 1] != '\t' && line[i + 1] != '\n')
// 		return (1);
// 	return (0);
// }

// static char	*apply_expansion(char *line, int start, int var_len, char *var_value)
// {
// 	char	*new_line;
// 	int		new_len;
// 	int		val_len;

// 	val_len = 0;
// 	if (var_value)
// 		val_len = ft_strlen(var_value);
// 	new_len = ft_strlen(line) - var_len + val_len;
// 	new_line = malloc(new_len + 1);
// 	if (!new_line)
// 		return (NULL);
// 	ft_strlcpy(new_line, line, start + 1);
// 	if (var_value)
// 		ft_strlcat(new_line, var_value, new_len + 1);
// 	ft_strlcat(new_line, line + start + var_len + 1, new_len + 1);
// 	free(line);
// 	return (new_line);
// }

// static char	*expand_env_var(char *line, int *i, t_cmd *cmd)
// {
// 	int		start;
// 	int		var_len;
// 	char	*var_name;
// 	char	*var_value;

// 	start = *i;
// 	(*i)++;
// 	var_len = 0;
// 	if (line[*i] == '?')
// 	{
// 		var_len = 1;
// 		var_value = ft_itoa(cmd->shell->exit_status);
// 	}
// 	else
// 	{
// 		while (line[*i + var_len] && (ft_isalnum(line[*i + var_len]) || 
// 				line[*i + var_len] == '_'))
// 			var_len++;
// 		if (var_len == 0)
// 			return (line);
// 		var_name = ft_substr(line, *i, var_len);
// 		var_value = get_env_value(var_name, cmd->shell->env);
// 		free(var_name);
// 	}
// 	*i = start - 1;
// 	return (apply_expansion(line, start, var_len + 1, var_value));
// }

void	read_heredoc_input(const char *delimiter, int fd, t_cmd *cmd, 
	bool expand_vars)
{
	char	*line;
	char	*processed;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			ft_putstr_fd("warning: here-document delimited by end-of-file\n", 2);
			break;
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break;
		}
		if (expand_vars && ft_strchr(line, '$'))
		{
			processed = process_line(line, cmd, expand_vars);
			ft_putendl_fd(processed, fd);
			free(processed);
		}
		else
			ft_putendl_fd(line, fd);
		free(line);
	}
}

char	*handle_expanded_line(char *arg, char *expanded_line)
{
	char	*new_line;

	new_line = ft_strdup(expanded_line);
	free(arg);
	return (new_line);
}

char	*process_line(char *arg, t_cmd *cmd, bool expand_vars)
{
	int		i;
	char	*expanded_line;
	char	*var_value;

	if (!expand_vars || !ft_strchr(arg, '$'))
		return (ft_strdup(arg));
	
	expanded_line = ft_strdup("");
	i = 0;
	while (arg[i])
	{
		if (arg[i] == '$')
		{
			var_value = expand_variable(arg, &i, cmd->shell);
			expanded_line = ft_strjoin_free(expanded_line, var_value);
		}
		else
		{
			expanded_line = ft_strjoin_char(expanded_line, arg[i]);
			i++;
		}
	}
	return (clean_empty_expansions(expanded_line));
}

// Helper function to join a string and a char
// static char	*ft_strjoin_char(char *str, char c)
// {
// 	char	*result;
// 	int		i;
// 	int		len;

// 	if (!str)
// 		len = 0;
// 	else
// 		len = ft_strlen(str);
	
// 	result = malloc(len + 2);
// 	if (!result)
// 		return (NULL);
	
// 	i = 0;
// 	while (i < len)
// 	{
// 		result[i] = str[i];
// 		i++;
// 	}
// 	result[i] = c;
// 	result[i + 1] = '\0';
	
// 	free(str);
// 	return (result);
// }

// // Helper function to join two strings and free the first one
// static char	*ft_strjoin_free(char *s1, char *s2)
// {
// 	char	*joined;
	
// 	joined = ft_strjoin(s1, s2);
// 	free(s1);
// 	free(s2);
// 	return (joined);
// }

char	*clean_empty_expansions(char *arg)
{
	char	*clean;
	int		i;
	int		j;

	clean = malloc(ft_strlen(arg) + 1);
	if (!clean)
		return (NULL);
	
	i = 0;
	j = 0;
	while (arg[i])
	{
		clean[j++] = arg[i++];
	}
	clean[j] = '\0';
	
	free(arg);
	return (clean);
}

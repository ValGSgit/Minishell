/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:19:03 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/14 11:21:33 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

const char	*get_random_temp_name(void)
{
	static char		temp_name[32];
	int				fd;
	unsigned char	random_bytes[6];
	int				i;

	ft_strlcpy(temp_name, HERE_TEMP, sizeof(temp_name));
	fd = open("/dev/urandom", O_RDONLY);
	if (fd == -1 || read(fd, random_bytes, 6) != 6)
	{
		perror("get_random_temp_name");
		if (fd != -1)
			close(fd);
		return (NULL);
	}
	close(fd);
	i = -1;
	while (++i < 6)
		temp_name[13 + i] = '0' + (random_bytes[i] % 10);
	temp_name[19] = '\0';
	return (temp_name);
}

char	*clean_empty_expansions(char *arg)
{
	char	*result;
	int		i;
	int		j;
	
	result = malloc(ft_strlen(arg) + 1);
	if (!result)
	return (arg);
	i = 0;
	j = 0;
	while (arg[i])
	{
		if (arg[i] == '$' && (arg[i + 1] == ' ' || arg[i + 1] == '\0' || arg[i
			+ 1] == '$' || arg[i + 1] == '/'))
			i++;
			else
			result[j++] = arg[i++];
	}
	result[j] = '\0';
	if (j < (int)ft_strlen(arg))
	{
		free(arg);
		return (result);
	}
	free(result);
	return (arg);
}

char	*process_line(char *arg, t_cmd *cmd, bool expand_vars)
{
	char	*expanded_line;

	if (!expand_vars)
		return (arg);
	expanded_line = process_argument(arg, cmd->shell);
	if (!expanded_line)
		return (arg);
	arg = handle_expanded_line(arg, expanded_line);
	return (clean_empty_expansions(arg));
}

char	*remove_quotes(char *lim)
{
	char	*new_lim;
	int		i;
	int		j;

	new_lim = malloc(sizeof(char) * (ft_strlen(lim) + 1));
	if (!new_lim)
		return (NULL);
	i = 0;
	j = 0;
	while (lim[i])
	{
		if (lim[i] != '"' && lim[i] != '\'')
			new_lim[j++] = lim[i];
		i++;
	}
	new_lim[j] = '\0';
	free(lim);
	return (new_lim);
}

void	cleanup_heredocs(t_shell *shell)
{
	t_redir	*redir;
	t_redir	*next;

	redir = shell->cmd->redirs;
	while (redir && redir->type == REDIR_HEREDOC)
	{
		if (redir->type == REDIR_HEREDOC && redir->file
			&& unlink(redir->file) == -1)
			perror("unlink");
		free(redir->file);
		next = redir->next;
		free(redir);
		redir = next;
	}
	shell->cmd->redirs = NULL;
}

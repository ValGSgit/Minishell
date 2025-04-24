/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs_extra_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 16:18:01 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/23 13:28:12 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* Track the temporary files we create */
static char *g_temp_files[256] = {NULL};
static int g_temp_count = 0;

void	add_temp_file(const char *filename)
{
	if (g_temp_count < 255 && filename)
	{
		g_temp_files[g_temp_count] = ft_strdup(filename);
		g_temp_count++;
	}
}

void	cleanup_temp_files(void)
{
	int	i;

	i = 0;
	while (i < g_temp_count)
	{
		if (g_temp_files[i])
		{
			unlink(g_temp_files[i]);
			xfree(g_temp_files[i]);
			g_temp_files[i] = NULL;
		}
		i++;
	}
	g_temp_count = 0;
}

const char	*get_random_temp_name(void)
{
	static char	template[] = HERE_TEMP;
	int			fd;

	fd = mkstemp(template);
	if (fd == -1)
		return (NULL);
	close(fd);
	add_temp_file(template);
	return (template);
}

char	*remove_quotes(char *lim)
{
	int		i;
	int		j;
	char	*clean_lim;

	if (!lim || (!ft_strchr(lim, '\'') && !ft_strchr(lim, '"')))
		return (ft_strdup(lim));
	clean_lim = xmalloc(ft_strlen(lim) + 1);
	if (!clean_lim)
		return (NULL);
	i = 0;
	j = 0;
	while (lim[i])
	{
		if (lim[i] != '\'' && lim[i] != '"')
			clean_lim[j++] = lim[i];
		i++;
	}
	clean_lim[j] = '\0';
	return (clean_lim);
}

/* setup_heredoc_signals function removed - using version from heredocs.c */

bool	check_delimiter_quotes(char *eof)
{
	bool	has_quotes;

	has_quotes = false;
	while (*eof)
	{
		if (*eof == '\'' || *eof == '"')
			has_quotes = true;
		eof++;
	}
	return (has_quotes);
}
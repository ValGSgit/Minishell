/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 11:54:20 by vagarcia          #+#    #+#             */
/*   Updated: 2024/10/23 12:51:32 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_freejoin(char *buffer, char *new)
{
	char	*tmp;

	tmp = ft_strjoin(buffer, new);
	if (!tmp)
		return (free(buffer), free(new), NULL);
	free(buffer);
	return (tmp);
}

char	*readfile(int fd, char *res)
{
	char	*buf;
	ssize_t	bytes;

	bytes = 1;
	if (!res)
	{
		res = ft_calloc(1, 1);
		if (!res)
			return (NULL);
	}
	buf = ft_calloc((BUFFER_SIZE + 1), sizeof(char));
	if (!buf)
		return (free(res), NULL);
	while (bytes > 0 && !ft_strchr(res, '\n'))
	{
		bytes = read(fd, buf, BUFFER_SIZE);
		if (bytes == -1)
			return (free(buf), free(res), NULL);
		buf[bytes] = 0;
		res = ft_freejoin(res, buf);
		if (!res)
			return (NULL);
	}
	return (free(buf), res);
}

char	*nextline(char	*buf)
{
	int		i;
	int		j;
	int		slen;
	char	*line;

	slen = 0;
	if (!buf || !buf[slen])
		return (free(buf), NULL);
	i = ft_strlen(buf);
	while (buf[slen] && buf[slen] != '\n')
		slen++;
	if (buf[slen] == '\n')
		slen++;
	if (i <= slen)
		return (free(buf), NULL);
	else
		line = ft_calloc((i - slen + 1), sizeof(char));
	if (!line)
		return (free(buf), NULL);
	j = 0;
	while (buf[slen])
		line[j++] = buf[slen++];
	if (buf[slen] == '\n')
		line[j] = buf[slen];
	return (free(buf), line);
}

char	*makeline(char	*buf)
{
	char	*line;
	int		i;

	i = 0;
	if (!buf || !buf[0])
		return (NULL);
	while (buf[i] != '\n' && buf[i])
		i++;
	if (buf[i] == '\n')
		i++;
	line = ft_calloc((i + 1), sizeof(char));
	if (!line)
		return (free(buf), NULL);
	i = 0;
	while (buf[i] && buf[i] != '\n')
	{
		line[i] = buf[i];
		i++;
	}
	if (buf[i] == '\n')
		line[i] = buf[i];
	return (line);
}

char	*get_next_line(int fd)
{
	static char	*buf[1024];
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || fd >= 1023)
		return (NULL);
	if (!ft_strchr(buf[fd], '\n'))
		buf[fd] = readfile(fd, buf[fd]);
	if (!buf[fd])
		return (free(buf[fd]), NULL);
	line = makeline(buf[fd]);
	if (!line)
	{
		free(buf[fd]);
		buf[fd] = NULL;
		return (NULL);
	}
	buf[fd] = nextline(buf[fd]);
	return (line);
}

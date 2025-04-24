/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 11:55:07 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/09 11:08:28 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// size_t	ft_strlen(const char *s)
// {
// 	size_t	i;

// 	if (!s)
// 		return (0);
// 	i = 0;
// 	while (s[i])
// 		i++;
// 	return (i);
// }

void	*bytecheck(ssize_t bytes, char *buf, char *line)
{
	if (bytes == 0)
		free(buf);
	if (bytes < 0)
	{
		free(buf);
		free(line);
	}
	return (NULL);
}

// char	*ft_strjoin(const char *s1, const char *s2)
// {
// 	char	*newstr;
// 	size_t	len1;
// 	size_t	len2;

// 	if (!s1 || !s2)
// 		return (NULL);
// 	newstr = malloc((ft_strlen(s1) + ft_strlen(s2) + 1) * sizeof(char));
// 	if (!newstr)
// 		return (NULL);
// 	len1 = 0;
// 	len2 = 0;
// 	while (len1 < ft_strlen(s1))
// 	{
// 		newstr[len1] = s1[len1];
// 		len1++;
// 	}
// 	while (len2 < ft_strlen(s2))
// 	{
// 		newstr[len1] = s2[len2];
// 		len1++;
// 		len2++;
// 	}
// 	newstr[len1] = '\0';
// 	return (newstr);
// }

// char	*ft_strchr(const char *str, int car)
// {
// 	char	*s;

// 	if (!str)
// 		return (NULL);
// 	s = (char *)str;
// 	while (*s != car && *s != '\0')
// 		s++;
// 	if (*s == car)
// 		return (s);
// 	else
// 		return (NULL);
// }

// void	*ft_calloc(size_t nmemb, size_t size)
// {
// 	char	*ptr;
// 	size_t	i;
// 	size_t	typ;

// 	typ = nmemb * size;
// 	if (size && ((typ / size) != nmemb))
// 		return (NULL);
// 	ptr = malloc(nmemb * size);
// 	if (!ptr)
// 		return (NULL);
// 	i = 0;
// 	while (i < typ)
// 	{
// 		ptr[i] = 0;
// 		i++;
// 	}
// 	return ((void *)ptr);
// }

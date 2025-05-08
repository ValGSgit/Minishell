/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 15:51:57 by vagarcia          #+#    #+#             */
/*   Updated: 2025/05/01 18:24:32 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_strcpy_join(char *dest, const char *s1, const char *s2)
{
	size_t	i;
	size_t	j;

	if (!dest)
		return (NULL);
	if (!s1)
		s1 = "";
	if (!s2)
		s2 = "";
	i = 0;
	while (s1[i] && s1[i] != '\0')
	{
		dest[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j] && s2[j] != '\0')
	{
		dest[i + j] = s2[j];
		j++;
	}
	dest[i + j] = '\0';
	return (dest);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*newstr;
	size_t	len1;
	size_t	len2;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	newstr = malloc(len1 + len2 + 1);
	if (!newstr)
		return (NULL);
	return (ft_strcpy_join(newstr, s1, s2));
}
/*
#include <stdio.h>




int	main(void)
{
	const char *s1 = "Para bailar la bamba,\n";
	const char *s2 = "se necesita una poca de gracia...";

	printf("If you see it then good job...\n");
	printf("%s\n", ft_strjoin(s1, s2));
}*/

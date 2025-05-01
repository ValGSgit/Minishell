/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 14:07:07 by vagarcia          #+#    #+#             */
/*   Updated: 2024/09/11 14:52:08 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"


static char	*ft_strcpy_join(char *dest, const char *s1, const char *s2,
	size_t len1, size_t len2)
{
size_t i;
size_t j;

i = 0;
while (i < len1)
{
	dest[i] = s1[i];
	i++;
}
j = 0;
while (j < len2)
{
	dest[i] = s2[j];
	i++;
	j++;
}
dest[i] = '\0';
return (dest);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
char *newstr;
size_t len1;
size_t len2;

if (!s1 || !s2)
	return (NULL);
len1 = ft_strlen(s1);
len2 = ft_strlen(s2);
newstr = malloc(len1 + len2 + 1);
if (!newstr)
	return (NULL);
return (ft_strcpy_join(newstr, s1, s2, len1, len2));
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

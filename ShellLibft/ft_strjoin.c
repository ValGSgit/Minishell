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

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*newstr;
	size_t	len1;
	size_t	len2;

	if (!s1 || !s2)
		return (NULL);
	newstr = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!newstr)
		return (NULL);
	len1 = 0;
	len2 = 0;
	while (len1 < ft_strlen(s1))
	{
		newstr[len1] = s1[len1];
		len1++;
	}
	while (len2 < ft_strlen(s2))
	{
		newstr[len1] = s2[len2];
		len1++;
		len2++;
	}
	newstr[len1] = '\0';
	return (newstr);
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 12:42:43 by vagarcia          #+#    #+#             */
/*   Updated: 2024/09/14 15:46:53 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

static int	setcheck(char c, const char *set)
{
	size_t	i;

	i = 0;
	while (set[i])
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*cpystr;
	size_t	i;
	size_t	strt;
	size_t	end;	

	strt = 0;
	while (s1[strt] && setcheck(s1[strt], set))
		strt++;
	end = ft_strlen(s1);
	while (end > strt && setcheck(s1[end - 1], set))
		end--;
	cpystr = malloc(sizeof(char) * (end - strt + 1));
	if (!cpystr)
		return (NULL);
	i = 0;
	while (strt < end)
		cpystr[i++] = s1[strt++];
	cpystr[i] = 0;
	return (cpystr);
}
/*
#include <stdio.h>
int	main(void)
{
	char const *dirtystr = "9861985619861 what if thi1s worked? 519812985610";
	char const *clean = "1234567890";

	printf("The clean string shows = %s\n", ft_strtrim(dirtystr, clean));
	return (0);
}*/

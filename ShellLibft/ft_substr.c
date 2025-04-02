/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 12:43:19 by vagarcia          #+#    #+#             */
/*   Updated: 2024/09/17 15:20:35 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*sub;
	size_t	maxb;
	int		memlen;
	int		i;

	if (!s || !*s || start >= ft_strlen(s))
		return (ft_strdup(""));
	maxb = (ft_strlen(s) - start);
	if (len < maxb)
		memlen = len;
	else
		memlen = maxb;
	sub = malloc(sizeof(char) * (memlen + 1));
	if (!sub)
		return (NULL);
	i = 0;
	while (i < memlen && s[i] != '\0')
		sub[i++] = s[start++];
	sub[i] = '\0';
	return (sub);
}
/*
#include <stdio.h>
int	main(void)
{
	char	*s = "hola mundo";
	size_t	len = 90000000;

	printf("Res is = %s\n", ft_substr(s, 0, len));
}*/

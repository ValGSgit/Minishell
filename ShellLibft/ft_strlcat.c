/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 13:01:26 by vagarcia          #+#    #+#             */
/*   Updated: 2024/09/22 15:05:43 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	len;
	size_t	dlen;

	len = 0;
	dlen = ft_strlen(dest);
	if (size == 0)
		return (ft_strlen(src));
	if (size <= dlen)
		return (size + ft_strlen(src));
	while (src[len] != '\0' && dlen + 1 < size)
	{
		dest[dlen] = src[len];
		len++;
		dlen++;
	}
	dest[dlen] = '\0';
	return (dlen + ft_strlen(&src[len]));
}
/*
#include <stdio.h>
#include <string.h>

int
main(void)
{
	const char *src = "This ";
	char *dest = "...as15";
	size_t totlen;

	totlen = ft_strlcat(dest, src, 20);
	printf("The result shows : %zu . From %s and %s", totlen, src, dest);
	return 0;
}*/

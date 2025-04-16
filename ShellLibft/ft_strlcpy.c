/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 15:18:33 by vagarcia          #+#    #+#             */
/*   Updated: 2025/03/31 15:19:21 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dest, const char *src, size_t size)
{
	size_t	src_len;
	size_t	i;

	src_len = 0;
	while (src[src_len])
		src_len++;
	if (size == 0)
		return (src_len);
	i = 0;
	while (src[i] && i < (size - 1))
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (src_len);
}
/*
#include <stdio.h>
#include <strings.h>
#include <unistd.h>

int	main(void) {
	char const *src = "abcd5678";
	char *dest = "1234";
	size_t len1;

	len1 = ft_strlcpy(dest, src, sizeof(dest));
	//size_t len2;
	//len2 = strlcpy(dest, src, sizeof(dest));
	printf("%s\n", src);
	printf("%s\n", dest);
	printf("ft = %zu\n", len1);
	//printf("strlcpy = %zu\n", len2);

	return(0);
}*/

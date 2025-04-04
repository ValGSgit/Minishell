/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 13:10:48 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/04 16:18:49 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	char	*ptr;
	size_t	i;
	size_t	typ;

	typ = nmemb * size;
	if (size && ((typ / size) != nmemb))
		return (NULL);
	ptr = malloc(nmemb * size);
	if (!ptr)
		return (NULL);
	i = 0;
	while (i < typ)
	{
		ptr[i] = 0;
		i++;
	}
	return ((void *)ptr);
}
// void	*ft_calloc(size_t nmemb, size_t size)
// {
// 	void	*ptr;
// 	size_t	typ;

// 	typ = nmemb * size;
// 	if (size && ((typ / size) != nmemb))
// 		return (NULL);
// 	ptr = malloc(typ);
// 	if (!ptr)
// 		return (NULL);
// 	ft_bzero(ptr, typ);
// 	return (ptr);
// }
/*
#include <stdlib.h>
#include <stdio.h>
int	main(void)
{
	printf("This is calloc %p\n", calloc(-1, -1));
	printf("This is ft_calloc %p\n", ft_calloc(-1, -1));
}*/

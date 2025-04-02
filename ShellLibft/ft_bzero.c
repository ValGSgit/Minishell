/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 12:54:50 by vagarcia          #+#    #+#             */
/*   Updated: 2024/09/10 17:14:36 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	ft_memset((void *)s, 0, (size_t) n);
}
/*
#include <stdio.h>
int	main(void)
{
	char *s = "cat";

	ft_bzero(s, 2);
	printf("The string said 'cat' yet bzero makes it = %s\n", s);
}*/

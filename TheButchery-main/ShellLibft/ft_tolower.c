/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolower.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 14:21:33 by vagarcia          #+#    #+#             */
/*   Updated: 2024/09/05 10:59:28 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

int	ft_tolower(int c)
{
	if (c >= 'A' && c <= 'Z')
	{
		c = c + 32;
	}
	return (c);
}
/*
#include <stdio.h>
int main(void)
{
    int test1 = 'a';
    int test2 = '1';
    int test3 = 'H';
    int test4 = 'q';

    printf("test1: %d\n", ft_tolower(test1));
    printf("test2: %d\n", ft_tolower(test2));
    printf("test3: %d\n", ft_tolower(test3));
    printf("test4: %d\n", ft_tolower(test4));
  return 0;
}*/

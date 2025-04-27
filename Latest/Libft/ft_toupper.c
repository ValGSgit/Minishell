/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 15:29:29 by vagarcia          #+#    #+#             */
/*   Updated: 2024/09/05 10:59:41 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

int	ft_toupper(int c)
{
	if (c >= 'a' && c <= 'z')
	{
		c = c - 32;
	}
	return (c);
}
/*
#include <stdio.h>
int main(void)
{
	int	test1 = 'a';
  	int	test2 = '1';
  	int	test3 = 'H';
  	int	test4 = 'q';

	printf("test1: %d\n", ft_toupper(test1));
  	printf("test2: %d\n", ft_toupper(test2));
  	printf("test3: %d\n", ft_toupper(test3));
  	printf("test4: %d\n", ft_toupper(test4));
  return 0;
}*/

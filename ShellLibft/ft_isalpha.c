/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 12:59:36 by vagarcia          #+#    #+#             */
/*   Updated: 2024/09/10 16:32:24 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

int	ft_isalpha(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1);
	else
		return (0);
}
/*
#include <stdio.h>
int main(void)
{
	char *test1 = "abc";
  	char *test2 = "123";
  	char *test3 = "";
  	char *test4 = "a1b";

	printf("test1: %d\n", ft_striteri(ft_isalpha(test1)));
  	printf("test2: %d\n", ft_isalpha(test2));
  	printf("test3: %d\n", ft_isalpha(test3));
  	printf("test4: %d\n", ft_isalpha(test4));
  return 0;
}*/

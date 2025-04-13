/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 12:59:18 by vagarcia          #+#    #+#             */
/*   Updated: 2024/09/19 19:14:06 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

int	ft_isalnum(int c)
{
	if (!((c >= 97 && c <= 122) || (c >= 65 && c <= 90)
			|| (c >= 48 && c <= 57)))
		return (0);
	return (1);
}
/*
#include <ctype.h>
#include <stdio.h>
int main(void)
{
	int test1 = '\0';
  	int test2 = '\0';
  	//char *test3 = "#+";
  	//char *test4 = "a1b";

	printf("test1: %d\n", isalnum(test1));
  	printf("test2: %d\n", ft_isalnum(test2));
  	//printf("test3: %d\n", ft_isalnum(test3));
  	//printf("test4: %d\n", ft_isalnum(test4));
  return 0;
}*/

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 13:17:42 by vagarcia          #+#    #+#             */
/*   Updated: 2024/09/05 10:39:41 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdint.h>

size_t	ft_strlen(const char *s)
{
	int	len;

	len = 0;
	if (!s)
		return (0);
	while (s[len] != '\0')
		len++;
	return (len);
}
/*
#include <stdio.h>
int main(void)
{
	char *test1 = "abßc";
  	char *test2 = "123";
  	char *test3 = "#+";
  	char *test4 = "a1b";

	printf("test1: %d\n", ft_strlen(test1));
  	printf("test2: %d\n", ft_strlen(test2));
  	printf("test3: %d\n", ft_strlen(test3));
  	printf("test4: %d\n", ft_strlen(test4));
  return 0;
}*/

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 12:59:53 by vagarcia          #+#    #+#             */
/*   Updated: 2025/03/31 15:22:45 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isdigit(int c)
{
	if (c == '\0')
		return (0);
	if (!(c >= '0' && c <= '9'))
		return (0);
	return (1);
}
/*
#include <stdio.h>
int main(void)
{
	char *test1 = "abc";
  	char *test2 = "123";
  	char *test3 = "";
  	char *test4 = "a1b";

	printf("test1: %d\n", ft_isdigit(test1));
  	printf("test2: %d\n", ft_isdigit(test2));
  	printf("test3: %d\n", ft_isdigit(test3));
  	printf("test4: %d\n", ft_isdigit(test4));
  return 0;
}*/

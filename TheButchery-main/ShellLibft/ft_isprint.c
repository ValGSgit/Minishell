/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 13:00:06 by vagarcia          #+#    #+#             */
/*   Updated: 2024/09/06 12:57:53 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

int	ft_isprint(int c)
{
	if (c == '\0')
		return (0);
	if (!(c >= 32 && c <= 126))
		return (0);
	return (1);
}
//ref, from 32 to 126 printable, below 32 and above 126 nonprint
/*
#include <stdio.h>
int main(void)
{
    char *test1 = "aöc";
    char *test2 = "123";
    char *test3 = "aAvasE";
    char *test4 = "ööö";

    printf("test1: %d\n", ft_isprint(test1));
    printf("test2: %d\n", ft_isprint(test2)); 
    printf("test3: %d\n", ft_isprint(test3)); 
    printf("test4: %d\n", ft_isprint(test4)); 
  return 0;
}*/

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 13:01:00 by vagarcia          #+#    #+#             */
/*   Updated: 2024/09/10 17:15:21 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		*(unsigned char *)(s + i) = (unsigned char) c;
		i++;
	}
	return (s);
}
/*
#include <stdio.h>
#include <string.h>
int main ()
{
  char str[] = "HHHHHHHHHHHHHHHHHHHHHHHHHHH";
  memset (str, 0,10);
  puts (str);
  char str2[] = "HHHHHHHHHHHHHHHHHHHHHHHHHHH";
  ft_memset(str2, 0,10);
  puts (str2);
  return 0;

}*/

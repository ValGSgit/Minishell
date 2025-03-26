/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 13:43:37 by vagarcia          #+#    #+#             */
/*   Updated: 2024/09/10 16:48:34 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t			i;
	int				diff;
	unsigned char	*hold1;
	unsigned char	*hold2;

	hold1 = (unsigned char *)s1;
	hold2 = (unsigned char *)s2;
	diff = 0;
	i = 0;
	if (n == 0)
		return (0);
	while (i < n - 1 && hold1[i] == hold2[i])
		i++;
	diff += (hold1[i] - hold2[i]);
	if (hold1[i] == hold2[i])
		return (0);
	if (hold1[i] < hold2[i] || hold1[i] > hold2[i])
		return (diff);
	return (0);
}
/*
 memcmp example
#include <stdio.h>
#include <string.h>

int main ()
{
  char buffer1[] = "m";
  char buffer2[] = "M";

  int n;

  n= ft_memcmp ( buffer1, buffer2, sizeof(buffer1) );

  if (n>0) printf ("'%s' is greater than '%s'.\n",buffer1,buffer2);
  else if (n<0) printf ("'%s' is less than '%s'.\n",buffer1,buffer2);
  else printf ("'%s' is the same as '%s'.\n",buffer1,buffer2);

  return 0;
}*/

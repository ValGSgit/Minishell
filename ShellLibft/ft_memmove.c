/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 14:04:54 by vagarcia          #+#    #+#             */
/*   Updated: 2024/09/09 16:23:51 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t				i;
	unsigned char		*holdest;
	const unsigned char	*holsrc;

	i = -1;
	holdest = (unsigned char *)dest;
	holsrc = (const unsigned char *)src;
	if (!dest && !src)
		return (NULL);
	if (holdest > holsrc && holdest < holsrc + n)
	{
		i = n + 1;
		while (--i > 0)
			holdest[i - 1] = holsrc[i - 1];
	}
	else
		while (++i < n)
			holdest[i] = holsrc[i];
	return (dest);
}

//#include <stdio.h>
//#include <string.h>
/*
//test1
int main () {
   char dest_str[] = "oldstring";
   const char src_str[]  = "newstring";
   printf("Before memmove dest = %s, src = %s\n", dest_str, src_str);
   ft_memmove(dest_str, src_str, 9);
   printf("After memmove dest = %s, src = %s\n", dest_str, src_str);
   return(0);
}
*/
//test2
/*
int main ()
{
  char str[] = "memmove can be very useful......";
  ft_memmove (str+20,str+15,11);
  printf("%s\n",str);
  return 0;
}
*/
//test3
/*
int main()
{
	char dest_str[] = "Tutorialspoint";
	char src_str[] = "Tutors";

	puts("source string [src_str] before memmove:-");
	puts(dest_str);

	// Copies contents from source to destination
	ft_memmove(dest_str, src_str, sizeof(src_str));

	puts("\nsource string [src_str] after memmove:-");
	puts(dest_str);
	return 0;
}*/

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 13:01:12 by vagarcia          #+#    #+#             */
/*   Updated: 2024/09/10 11:15:27 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	while ((char)c != *s)
	{
		if (!*s)
			return (0);
		s++;
	}
	return ((char *)s);
}
/*
#include <stdio.h>
#include <string.h>
int main ()
{
   const char *str = "yourmissionistofindMe and what comes after";
   const char ch = 'M';
   char *ret;
   char *ret2;

   ret = strchr(str, ch);
   ret2 = ft_strchr(str, ch);
   printf("String after |%c| is - |%s|\n", ch, ret);
   printf("ft_String after |%c| is - |%s|\n", ch, ret2);
   return(0);
}*/

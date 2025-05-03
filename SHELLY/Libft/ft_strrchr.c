/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 10:37:54 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/30 20:23:58 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int	i;

	if (!s)
		return (0);
	i = ft_strlen(s);
	while (i > 0 && s[i] != (char)c)
		i--;
	if (s[i] == (char)c)
		return ((char *)s + i);
	else
		return (0);
}
/*
#include <stdio.h>
#include <string.h>
int main ()
{
   const char *str = "yourmissionistofinde and what cos after";
   const char ch = 'M';
   char *ret;
   char *ret2;

   ret = strrchr(str, ch);
   ret2 = ft_strrchr(str, ch);
   printf("String after |%c| is - |%s|\n", ch, ret);
   printf("ft_String after |%c| is - |%s|\n", ch, ret2);
   return(0);
}*/

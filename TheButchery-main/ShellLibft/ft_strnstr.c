/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 16:33:36 by vagarcia          #+#    #+#             */
/*   Updated: 2024/10/02 17:52:09 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	int		lilcount;

	i = 0;
	if (!*little)
		return ((char *)big);
	while (big[i])
	{
		lilcount = 0;
		while (big[i] == little[lilcount] && big[i] && i < len)
		{
			i++;
			lilcount++;
		}
		if (little[lilcount] == '\0')
			return ((char *)&big[i - lilcount]);
		i = (i - lilcount) + 1;
	}
	return (NULL);
}
/*
#include <stdio.h>

int main(void)
{

	const char *largestring = "oasfaaoiuysfgWaldoahbnasdasgagsag";
	const char *smallstring = "Waldo";
	char *ptr;
	
	ptr = ft_strnstr(largestring, smallstring, 8);
	printf("The output is ... %s", ptr);
}*/

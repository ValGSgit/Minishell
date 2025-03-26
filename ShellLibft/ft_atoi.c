/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 12:59:07 by vagarcia          #+#    #+#             */
/*   Updated: 2024/09/14 16:26:27 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

static int	ft_skipspace(const char *str)
{
	int	i;

	i = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == ' ')
		i++;
	return (i);
}

int	ft_atoi(const char *str)
{
	int	i;
	int	neg;
	int	res;

	i = ft_skipspace(str);
	res = 0;
	neg = 1;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			neg *= -1;
		i++;
	}
	while (str[i] != '\0')
	{
		if (str[i] >= '0' && str[i] <= '9')
		{
			res *= 10;
			res += (int)(str[i] - 48);
			i++;
		}
		else
			return (neg * res);
	}
	return (neg * res);
}
/*
#include <stdio.h>
#include <stdlib.h>
int main()
{
	printf("1ft_atoi: %d\n", ft_atoi("  --+-123456"));
	printf("atoi: %d\n", atoi("  -+--123456"));
	printf("2ft_atoi: %d\n", ft_atoi("12Three45678"));
	printf("atoi: %d\n", atoi("12Three45678"));
	printf("3ft_atoi: %d\n", ft_atoi("Hello World!"));
	printf("atoi: %d\n", atoi("Hello World!"));
	printf("4ft_atoi: %d\n", ft_atoi("-2147483647 BLAH!"));
	printf("atoi: %d\n", atoi("-2147483647 BLAH!"));
	printf("5ft_atoi: %d\n", ft_atoi("-42"));
	printf("atoi: %d\n", atoi("-42"));
	printf("6ft_atoi: %d\n", ft_atoi("     +42"));
	printf("atoi: %d\n", atoi("     +42"));
	printf("7ft_atoi: %d\n", ft_atoi("\t\n\v\f\r 42"));
	printf("atoi: %d\n", atoi("\t\n\v\f\r 42"));
	printf("8ft_atoi: %d\n", ft_atoi("  5"));
	printf("atoi: %d\n", atoi("  5"));
	return 0;
}*/

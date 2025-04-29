/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 16:46:52 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/09 12:37:21 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	numlen(int n)
{
	int	count;

	if (n == 0)
		return (1);
	count = 0;
	while (n)
	{
		count++;
		n /= 10;
	}
	return (count);
}

char	*ft_itoa(int n)
{
	char	*nbstr;
	int		len;
	long	num;

	if (n == 0)
		return (ft_strdup("0"));
	num = n;
	len = numlen(n);
	if (len == 0)
		len = 1;
	nbstr = (char *)ft_calloc(sizeof(char), (len + 1));
	if (!nbstr)
		return (NULL);
	nbstr[len] = '\0';
	if (num < 0)
	{
		nbstr[0] = '-';
		num = -num;
	}
	while (--len >= 0 && nbstr[len] != '-')
	{
		nbstr[len] = '0' + (num % 10);
		num /= 10;
	}
	return (nbstr);
}

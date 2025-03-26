/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 18:03:50 by vagarcia          #+#    #+#             */
/*   Updated: 2024/09/09 18:15:00 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	ft_putendl_fd(char *s, int fd)
{
	int	i;

	i = 0;
	if (!s)
		return ;
	while (s[i])
	{
		ft_putchar_fd(s[i], fd);
		i++;
	}
	ft_putstr_fd("\n", fd);
}
/*
#include <stdio.h>

int main(void)
{
	char *s = "This would be first and separated by";
	char *s1 = "a new line here.";
	
	ft_putendl_fd(s, 1);
	ft_putendl_fd(s1, 1);
}*/

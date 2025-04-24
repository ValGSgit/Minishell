/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 17:17:36 by vagarcia          #+#    #+#             */
/*   Updated: 2024/09/13 11:51:40 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	ft_putnbr_fd(int nb, int fd)
{
	if (nb == -2147483648)
	{
		write(fd, "-2147483648", 11);
		return ;
	}
	if (nb < 0)
	{
		ft_putchar_fd('-', fd);
		nb = -nb;
	}
	if (nb >= 10)
	{
		ft_putnbr_fd(nb / 10, fd);
		nb = nb % 10;
	}
	if (nb < 10)
	{
		ft_putchar_fd(nb + 48, fd);
	}
}
/*
#include <stdio.h>
int	main(void)
{
	int	t1;
	int t2;
	int t3;
	int	fd;


	fd = 1;
	t1 = 2147483647;
	t2 = -15125112;
	t3 = 699669;
	ft_putnbr_fd(t1, fd);
	ft_putnbr_fd(t2, fd);
	ft_putnbr_fd(t3, fd);
}*/

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 21:35:02 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/27 10:30:00 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/memory.h"

// volatile sig_atomic_t	g_signal_received = 0;

void	*xmalloc(size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
	{
		ft_putstr_fd("No problem \n", 2);
	}
	return (ptr);
}

void	safe_free(void *ptr)
{
	if (ptr)
		free(ptr);
}
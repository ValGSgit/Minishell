/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_manager.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 11:15:00 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/23 11:15:00 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * Allocates memory and tracks it
 */
void	*xmalloc(size_t size)
{
	void	*ptr;

	ptr = tracked_malloc(size);
	if (!ptr)
	{
		clean_all_memory();
		exit(1);
	}
	return (ptr);
}

/**
 * Frees memory and removes it from tracking
 */
void	xfree(void *ptr)
{
	if (!ptr)
		return ;
	tracked_free(ptr);
}

/**
 * Cleans up all tracked memory
 */
void	clean_memory(void)
{
	clean_all_memory();
}

/**
 * Frees memory but marks it as manually freed so clean_all_memory
 * won't try to free it again
 */
void	xfree_manual(void *ptr)
{
	if (!ptr)
		return ;
	/* Remove from tracking but still free the memory */
	mark_no_free(ptr);
	free(ptr);
}
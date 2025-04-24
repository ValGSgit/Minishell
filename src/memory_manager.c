/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_manager.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 11:15:00 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/24 23:30:00 by vagarcia         ###   ########.fr       */
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
	/* This will call clean_all_memory from malloc_tracker.c */
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

/**
 * Free memory safely, checking if it's already freed
 */
void	safe_free(void *ptr)
{
	if (!ptr)
		return ;
	
	/* Check if the pointer is tracked */
	if (find_memblock(ptr))
		xfree(ptr);
	/* Otherwise, it's likely been freed already or wasn't tracked */
}
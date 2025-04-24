/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xmalloc_tracker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 17:20:00 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/23 10:35:00 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/malloc_tracker.h"
#include "../includes/minishell.h"

/* Static list variable for memory tracking in this module */
static t_memblock	*g_memtrack = NULL;

/**
 * Getter for the memory tracking list
 */
t_memblock	*get_memtrack_list(void)
{
	return (g_memtrack);
}

/**
 * Setter for the memory tracking list
 */
void	set_memtrack_list(t_memblock *list)
{
	g_memtrack = list;
}

/**
 * Adds a new memory block to the tracking list
 */
void	add_memblock(void *ptr, size_t size)
{
	t_memblock	*new_block;

	new_block = malloc(sizeof(t_memblock));
	if (!new_block)
		return ;
	new_block->ptr = ptr;
	new_block->size = size;
	new_block->next = g_memtrack;
	g_memtrack = new_block;
}

/**
 * Finds a memory block in the tracking list
 */
t_memblock	*find_memblock(void *ptr)
{
	t_memblock	*current;

	current = g_memtrack;
	while (current)
	{
		if (current->ptr == ptr)
			return (current);
		current = current->next;
	}
	return (NULL);
}

/**
 * Removes a memory block from the tracking list
 */
void	remove_memblock(void *ptr)
{
	t_memblock	*current;
	t_memblock	*prev;

	current = g_memtrack;
	prev = NULL;
	while (current)
	{
		if (current->ptr == ptr)
		{
			if (prev)
				prev->next = current->next;
			else
				g_memtrack = current->next;
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

/**
 * Tracked malloc function that records allocations
 */
void	*tracked_malloc(size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	if (ptr)
		add_memblock(ptr, size);
	return (ptr);
}

/**
 * Tracked free function that removes allocations from tracking
 */
void	tracked_free(void *ptr)
{
	t_memblock	*block;
	
	if (!ptr)
		return ;
	block = find_memblock(ptr);
	if (!block)
	{
		/* Still free the memory even if it wasn't tracked */
		free(ptr);
	}
	else
	{
		remove_memblock(ptr);
		free(ptr);
	}
}

/**
 * Counts the number of active allocations
 */
int	count_allocations(void)
{
	t_memblock	*current;
	int			count;

	current = g_memtrack;
	count = 0;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

/**
 * Prints any memory leaks at program exit (optional debugging function)
 */
void	print_memory_leaks(void)
{
	t_memblock	*current;
	int			count;

	current = g_memtrack;
	count = 0;
	while (current)
	{
		/* Removed file/line info as it's no longer tracked */
		count++;
		current = current->next;
	}
	//if (count > 0)
		//fprintf(stderr, "Total leaks: %d\n", count);
}

/**
 * Mark a pointer to be excluded from cleaning
 */
void	mark_no_free(void *ptr)
{
	t_memblock	*block;

	if (!ptr)
		return ;
	block = find_memblock(ptr);
	if (block)
		remove_memblock(ptr);
}

/**
 * Cleans up all tracked memory
 */
void	clean_all_memory(void)
{
	t_memblock	*current;
	t_memblock	*next;
	void		*ptr_to_free;

	current = g_memtrack;
	while (current)
	{
		next = current->next;
		ptr_to_free = current->ptr;
		/* Remove from tracker before freeing to avoid invalid access */
		remove_memblock(ptr_to_free);
		if (ptr_to_free)
			free(ptr_to_free);
		/* Don't free 'current' again as it's already freed in remove_memblock */
		current = next;
	}
	g_memtrack = NULL;
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_tracker.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 17:20:00 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/23 10:30:00 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_TRACKER_H
# define MALLOC_TRACKER_H

# include <stdlib.h>
# include <stdio.h>

typedef struct s_memblock
{
	void				*ptr;
	size_t				size;
	struct s_memblock	*next;
}	t_memblock;

/* Memory tracking functions */
void		*tracked_malloc(size_t size);
int		tracked_free(void *ptr);
void		clean_all_memory(void);
int			count_allocations(void);
void		print_memory_leaks(void);
t_memblock	*get_memtrack_list(void);
void		set_memtrack_list(t_memblock *list);
t_memblock	*find_memblock(void *ptr);
int		remove_memblock(void *ptr);
void		add_memblock(void *ptr, size_t size);
void		mark_no_free(void *ptr);

#endif
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 21:30:22 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/30 10:25:00 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MEMORY_H
# define MEMORY_H

# include <stdlib.h>
# include <stdbool.h>
# include <signal.h>

/* Only allowed global variable for signals */
extern volatile sig_atomic_t	g_signal_received;

/* Memory management functions */
void	*xmalloc(size_t size);
void	safe_free(void *ptr);

#endif
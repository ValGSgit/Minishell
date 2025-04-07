/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 11:55:07 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/07 16:09:55 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*bytecheck(ssize_t bytes, char *buf, char *line)
{
	if (bytes == 0)
		free(buf);
	if (bytes < 0)
	{
		free(buf);
		free(line);
	}
	return (NULL);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 21:35:02 by vagarcia          #+#    #+#             */
/*   Updated: 2025/05/01 10:15:23 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	safe_free(void *ptr)
{
	if (ptr)
		free(ptr);
	ptr = NULL;
}

void	free_ptr_array(void **array)
{
	int	i;

	i = 0;
	if (!array)
		return ;
	while (array[i])
	{
		safe_free(array[i]);
		i++;
	}
	safe_free(array);
}

char	*safe_strjoin(char *s1, char *s2, int free_flag)
{
	char	*result;

	result = ft_strjoin(s1, s2);
	if (!result)
	{
		if (free_flag == 1 || free_flag == 3)
			safe_free(s1);
		if (free_flag == 2 || free_flag == 3)
			safe_free(s2);
		return (NULL);
	}
	if (free_flag == 1 || free_flag == 3)
		safe_free(s1);
	if (free_flag == 2 || free_flag == 3)
		safe_free(s2);
	return (result);
}

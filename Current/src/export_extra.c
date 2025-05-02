/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_extra.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iionescu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 09:19:40 by iionescu          #+#    #+#             */
/*   Updated: 2025/05/02 09:19:44 by iionescu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_sort_strings(char **arr, int count)
{
	int		i;
	int		j;
	int		swapped;
	char	*temp;

	i = 0;
	while (i < count - 1)
	{
		swapped = 0;
		j = 0;
		while (j < count - i - 1)
		{
			if (ft_strcmp(arr[j], arr[j + 1]) > 0)
			{
				temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
				swapped = 1;
			}
			j++;
		}
		if (swapped == 0)
			break ;
		i++;
	}
}

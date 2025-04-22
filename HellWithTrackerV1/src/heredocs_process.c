/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs_process.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 23:20:15 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/22 23:20:15 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Helper function to join a string and a char
char	*ft_strjoin_char(char *str, char c)
{
	char	*result;
	int		i;
	int		len;

	if (!str)
		len = 0;
	else
		len = ft_strlen(str);
	
	result = malloc(len + 2);
	if (!result)
		return (NULL);
	
	i = 0;
	while (i < len)
	{
		result[i] = str[i];
		i++;
	}
	result[i] = c;
	result[i + 1] = '\0';
	
	free(str);
	return (result);
}

// Helper function to join two strings and free the first one
char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*joined;
	
	joined = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (joined);
}
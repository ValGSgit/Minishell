/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 16:22:19 by vagarcia          #+#    #+#             */
/*   Updated: 2025/04/04 16:50:21 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t			i;
	const unsigned char	*cs1 = (const unsigned char *)s1;
	const unsigned char	*cs2 = (const unsigned char *)s2;

	if (n == 0)
		return (0);
	i = 0;
	while (i < n - 1 && cs1[i] == cs2[i] && cs1[i] != '\0' && cs2[i] != '\0')
		i++;
	return (cs1[i] - cs2[i]);
}

// int	ft_strncmp(const char *s1, const char *s2, size_t n)
// {
// 	size_t			i;
// 	unsigned char	*cs1;
// 	unsigned char	*cs2;

// 	cs1 = (unsigned char *)s1;
// 	cs2 = (unsigned char *)s2;
// 	if (n == 0)
// 		return (0);
// 	i = 0;
// 	while (cs1[i] == cs2[i] && cs2[i] != '\0' && i + 1 < n)
// 		i++;
// 	if (cs1[i] == cs2[i])
// 		return (0);
// 	else
// 		return (cs1[i] - cs2[i]);
// }
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 13:17:33 by vagarcia          #+#    #+#             */
/*   Updated: 2025/03/24 14:17:36 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dest, const char *src, size_t size)
{
    size_t	src_len;
    size_t	i;

    src_len = 0;
    while (src[src_len]) // Calculate the length of the source string
        src_len++;
    if (size == 0) // If size is 0, just return the length of the source string
        return (src_len);
    i = 0;
    while (src[i] && i < (size - 1)) // Copy up to size - 1 characters
    {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0'; // Null-terminate the destination string
    return (src_len); // Return the total length of the source string
}
/*
#include <unistd.h>
#include <strings.h>
#include <stdio.h>
int main() {
    char const *src = "abcd5678";
    char *dest = "1234";
	size_t len1;

	len1 = ft_strlcpy(dest, src, sizeof(dest));
	//size_t len2;
	//len2 = strlcpy(dest, src, sizeof(dest));
    printf("%s\n", src);
    printf("%s\n", dest);
    printf("ft = %zu\n", len1);
	//printf("strlcpy = %zu\n", len2);

    return(0);
}*/

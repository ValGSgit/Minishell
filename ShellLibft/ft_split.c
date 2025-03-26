/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 18:39:05 by vagarcia          #+#    #+#             */
/*   Updated: 2024/09/20 11:53:46 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_wc(const char *s, char c)
{
	int	i;
	int	trigger;

	i = 0;
	trigger = 0;
	while (*s)
	{
		if (*s != c && trigger == 0)
		{
			trigger = 1;
			i++;
		}
		else if (*s == c)
			trigger = 0;
		s++;
	}
	return (i);
}

static void	ft_free(char **arr)
{
	int		i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

// arr[1] == *(arr + 1)
// arr = 1 arr += 1 arr = 2 arr 
static char	*wordup(const char *s, int start, int finish)
{
	char	*word;
	int		i;

	i = 0;
	word = malloc(sizeof(char) * (finish - start + 1));
	if (!word)
		return (NULL);
	while (start < finish)
		word[i++] = s[start++];
	word[i] = '\0';
	return (word);
}

static void	*ft_woodchipper(char const *s, char **tab, char c)
{
	size_t	i;
	int		j;
	int		beg;

	i = 0;
	j = 0;
	beg = -1;
	while (j < ft_wc(s, c))
	{
		if (s[i] != c && beg < 0)
			beg = i;
		else if ((s[i] == c || i == ft_strlen(s)) && beg >= 0)
		{
			tab[j] = wordup(s, beg, i);
			if (!tab[j])
				return (ft_free(tab), NULL);
			j++;
			beg = -1;
		}
		i++;
	}
	tab[j] = NULL;
	return (tab);
}

char	**ft_split(char const *s, char c)
{
	char	**tab;

	tab = malloc((sizeof(char *) * (ft_wc(s, c) + 1)));
	if (!s || !tab)
		return (NULL);
	tab = ft_woodchipper(s, tab, c);
	return (tab);
}
/*
#include <stdio.h>

int	main(void)
{
	char **tabstr;
	int			i;

	i = 0;
	tabstr = ft_split("Once \0upon a \0time, there was\0 ll test...", '\0');
   	while (tabstr[i] != NULL)
	{
		printf("%s\n", tabstr[i]);
		i++;
	}
}*/

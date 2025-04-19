/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 14:11:04 by vagarcia          #+#    #+#             */
/*   Updated: 2024/09/20 12:05:38 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void*(*f)(void*), void (*del)(void *))
{
	t_list	*fnode;
	t_list	*new;
	void	*content_new;

	if (lst != NULL && f != NULL)
	{
		fnode = 0;
		while (lst)
		{
			content_new = f(lst->content);
			if (!content_new)
				return (ft_lstclear(&fnode, del), NULL);
			new = ft_lstnew(content_new);
			if (new == NULL)
			{
				del(content_new);
				ft_lstclear(&fnode, del);
				return (NULL);
			}
			ft_lstadd_back(&fnode, new);
			lst = lst->next;
		}
		return (fnode);
	}
	return (NULL);
}

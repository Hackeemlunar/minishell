/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 17:04:29 by hmensah-          #+#    #+#             */
/*   Updated: 2025/02/15 18:45:43 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_list.h"

static int	ft_lstappend(t_list **list, t_list *new_node)
{
	t_list	*tail;

	if (!list || !new_node)
		return (0);
	if (!*list)
		*list = new_node;
	else
	{
		tail = *list;
		while (tail->next)
			tail = tail->next;
		tail->next = new_node;
	}
	return (1);
}

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_list;
	t_list	*new_node;

	if (!lst || !f)
		return (NULL);
	new_list = NULL;
	while (lst)
	{
		new_node = ft_lstnew(f(lst->content));
		if (!new_node)
		{
			ft_lstclear(&new_list, del);
			return (NULL);
		}
		if (!ft_lstappend(&new_list, new_node))
		{
			ft_lstclear(&new_list, del);
			return (NULL);
		}
		lst = lst->next;
	}
	return (new_list);
}

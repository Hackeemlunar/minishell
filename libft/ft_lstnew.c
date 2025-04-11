/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 17:35:25 by hmensah-          #+#    #+#             */
/*   Updated: 2025/02/15 18:45:47 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_list.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*linked_list;

	linked_list = (t_list *) malloc(sizeof(t_list));
	if (!linked_list)
		return (NULL);
	linked_list->content = content;
	linked_list->next = NULL;
	return (linked_list);
}

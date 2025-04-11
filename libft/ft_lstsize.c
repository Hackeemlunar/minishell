/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 19:58:59 by hmensah-          #+#    #+#             */
/*   Updated: 2025/02/15 18:45:58 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_list.h"

int	ft_lstsize(t_list *lst)
{
	t_list	*first;
	int		size;

	first = lst;
	size = 0;
	while (first != NULL)
	{
		size++;
		first = first->next;
	}
	return (size);
}

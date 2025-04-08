/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 18:49:46 by hmensah-          #+#    #+#             */
/*   Updated: 2024/12/27 21:35:18 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *b, int c, size_t len)
{
	char	*s_copy;
	int		i;

	i = 0;
	s_copy = (char *)b;
	while (i < (int)len)
	{
		s_copy[i] = c;
		i++;
	}
	return (b);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atod.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 16:17:19 by hmensah-          #+#    #+#             */
/*   Updated: 2025/03/10 20:15:24 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

double	ft_atod(const char *str)
{
	double	res;
	long	main_part;
	long	dec_part;
	int		counter;
	int		temp;

	main_part = ft_atol(str);
	counter = 0;
	temp = 1;
	while ((str[counter] != '.'))
		counter++;
	if (str[counter] == '.')
	{
		dec_part = ft_atol(&str[++counter]);
		while (str[counter] && str[counter] >= '0' && str[counter] <= '9')
		{
			counter++;
			temp *= 10;
		}
		res = main_part + (double)dec_part / temp;
	}
	else
		res = main_part;
	return (res);
}

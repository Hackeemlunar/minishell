/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 19:00:52 by hmensah-          #+#    #+#             */
/*   Updated: 2024/12/27 21:34:03 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_isspace(char c)
{
	return (c == ' ' || c == '\n' || c == '\f' || c == '\r'
		|| c == '\v' || c == '\t');
}

static long	get_number(const char *str, int sign)
{
	long	res;
	int		counter;

	res = 0;
	counter = 0;
	while (str[counter] >= '0' && str[counter] <= '9')
	{
		if (res > (LONG_MAX - (str[counter] - '0')) / 10)
		{
			if (sign == 1)
				return (-1);
			else
				return (0);
		}
		res = (res * 10) + (str[counter] - '0');
		counter++;
	}
	return (sign * res);
}

long	ft_atol(const char *str)
{
	int		sign;
	int		counter;

	sign = 1;
	counter = 0;
	while (ft_isspace(str[counter]))
		counter++;
	if (str[counter] == '-' || str[counter] == '+')
	{
		if (str[counter] == '-')
			sign = -1;
		counter++;
	}
	return (get_number(&str[counter], sign));
}

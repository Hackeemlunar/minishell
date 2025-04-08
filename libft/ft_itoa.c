/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 16:09:25 by hmensah-          #+#    #+#             */
/*   Updated: 2024/12/27 21:34:54 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*handle_special_cases(int n)
{
	char	*res;

	if (n == 0)
	{
		res = (char *)malloc(2);
		if (!res)
			return (NULL);
		res[0] = '0';
		res[1] = '\0';
		return (res);
	}
	if (n == INT_MIN)
	{
		res = (char *)malloc(13);
		if (!res)
			return (NULL);
		ft_strlcpy(res, "-2147483648", 13);
		return (res);
	}
	return (NULL);
}

static char	*convert_number(int n, int is_negative)
{
	char	temp[12];
	char	*res;
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (n > 0)
	{
		temp[i++] = (n % 10) + '0';
		n /= 10;
	}
	res = (char *)malloc(i + is_negative + 1);
	if (!res)
		return (NULL);
	if (is_negative)
		res[j++] = '-';
	while (i > 0)
		res[j++] = temp[--i];
	res[j] = '\0';
	return (res);
}

char	*ft_itoa(int n)
{
	char	*special_case_res;
	int		is_negative;

	is_negative = 0;
	special_case_res = handle_special_cases(n);
	if (special_case_res)
		return (special_case_res);
	if (n < 0)
	{
		is_negative = 1;
		n = -n;
	}
	return (convert_number(n, is_negative));
}

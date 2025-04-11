/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_width_flags.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 13:51:36 by hmensah-          #+#    #+#             */
/*   Updated: 2025/02/15 18:02:52 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	all_are_zeroes(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] != '0')
			return (0);
		i++;
	}
	return (1);
}

void	add_prefix(const char *pref, t_fdata *data)
{
	char	*str;
	int		wid;

	wid = data->count + 2;
	str = malloc(wid + 1);
	ft_strcpy(str, pref);
	ft_strcpy(str + 2, data->fstring);
	str[wid] = '\0';
	free(data->fstring);
	data->fstring = str;
	data->count = wid;
}

void	pad(t_modinfo *info, t_fdata *data, int width)
{
	char	*str;

	str = malloc(width + 1);
	if (ft_strchr(info->flags, '-'))
	{
		ft_strcpy(str, data->fstring);
		ft_memset(str + data->count, ' ', width - data->count);
	}
	else if (ft_strchr(info->flags, '0') && info->precision < 0)
	{
		ft_strlcpy(str, data->fstring, 3);
		ft_memset(str + 2, '0', width - data->count);
		ft_strcpy(str + 2 + (width - data->count), data->fstring + 2);
	}
	else
	{
		ft_memset(str, ' ', width - data->count);
		ft_strcpy(str + (width - data->count), data->fstring);
	}
	str[width] = '\0';
	free(data->fstring);
	data->fstring = str;
	data->count = width;
}

static void	fill_with_zeroes(t_modinfo *info, t_fdata *data)
{
	char	*str;

	str = malloc(info->width + 1);
	ft_memset(str, '0', info->width);
	str[info->width] = '\0';
	free(data->fstring);
	data->fstring = str;
	data->count = info->width;
}

void	apply_prefix(const char *pref, t_modinfo *info, t_fdata *data)
{
	int		all_zeroes;

	all_zeroes = all_are_zeroes(data->fstring);
	if (!all_zeroes)
	{
		add_prefix(pref, data);
	}
	if (info->width > data->count)
	{
		if (all_zeroes && (info->precision < 0) && ft_strchr(info->flags, '0'))
			fill_with_zeroes(info, data);
		else
			pad(info, data, info->width);
	}
}

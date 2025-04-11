/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   width_flags_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 15:48:58 by hmensah-          #+#    #+#             */
/*   Updated: 2025/02/15 18:03:12 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	apply_left_justify(char *fstring, int width, t_fdata *fdata)
{
	char	*new_string;
	int		padding;

	new_string = (char *)ft_calloc(width + 1, sizeof(char));
	if (!new_string)
		return ;
	padding = width - fdata->count;
	ft_memcpy(new_string, fstring, fdata->count);
	ft_memset(new_string + fdata->count, ' ', padding);
	new_string[width] = '\0';
	free(fstring);
	fdata->fstring = new_string;
	fdata->count = width;
}

void	apply_r_justify(char *fstring, int width, char pad, t_fdata *fdata)
{
	char	*new_string;
	int		padding;

	new_string = (char *)ft_calloc(width + 1, sizeof(char));
	if (!new_string)
		return ;
	padding = width - fdata->count;
	ft_memset(new_string, pad, padding);
	ft_memcpy(new_string + padding, fstring, fdata->count);
	free(fstring);
	fdata->fstring = new_string;
	fdata->count = width;
}

static void	apply_r_jfy_0(int width, char pad, t_fdata *fdata)
{
	char	*str;

	str = malloc(width + 1);
	if (!str)
		return ;
	str[0] = fdata->fstring[0];
	ft_memset(str + 1, pad, width - fdata->count);
	ft_strcpy(str + 1 + (width - fdata->count), fdata->fstring + 1);
	str[width] = '\0';
	free(fdata->fstring);
	fdata->fstring = str;
	fdata->count = width;
}

void	apply_minus_flag(t_modinfo *info, t_fdata *data)
{
	if (info->width > data->count)
	{
		if (ft_strchr(info->flags, '-'))
			apply_left_justify(data->fstring, info->width, data);
		else if (ft_strchr(info->flags, '0') && info->precision <= 0)
		{
			if ((data->fstring[0] == '+' || data->fstring[0] == '-'
					|| data->fstring[0] == ' ') && info->precision == -1)
				apply_r_jfy_0(info->width, '0', data);
			else if (info->precision == 0)
				apply_r_justify(data->fstring, info->width, ' ', data);
			else
				apply_r_justify(data->fstring, info->width, '0', data);
		}
		else
			apply_r_justify(data->fstring, info->width, ' ', data);
	}
}

void	apply_plus_space_flag(t_modinfo *info, t_fdata *data)
{
	char	*new_string;

	new_string = data->fstring;
	if (ft_strchr(info->flags, '+') && data->fstring[0] != '-')
	{
		data->count++;
		new_string = ft_strjoin("+", data->fstring);
		free(data->fstring);
		data->fstring = new_string;
	}
	else if (ft_strchr(info->flags, ' ') && data->fstring[0] != '-')
	{
		data->count++;
		new_string = ft_strjoin(" ", data->fstring);
		free(data->fstring);
		data->fstring = new_string;
	}
}

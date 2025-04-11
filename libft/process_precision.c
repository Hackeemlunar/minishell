/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_precision.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 17:03:18 by hmensah-          #+#    #+#             */
/*   Updated: 2025/02/15 18:02:32 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	handle_string_size(char *fstring, int size, t_fdata *data)
{
	int	len;

	len = data->count;
	if (len > size)
	{
		fstring[size] = '\0';
		data->count = size;
	}
}

void	handle_int_size(char *fstring, char pad, int size, t_fdata *data)
{
	size_t	padding;
	char	*new_string;

	if (size >= data->count)
	{
		if (*fstring == '-')
			size++;
		padding = size - data->count;
		new_string = (char *)malloc(size + 1);
		if (!new_string)
			return ;
		ft_memset(new_string, pad, size);
		new_string[size] = '\0';
		if (*fstring == '-')
		{
			new_string[0] = '-';
			ft_memmove(new_string + padding + 1, fstring + 1, data->count - 1);
		}
		else
			ft_memmove(new_string + padding, fstring, data->count);
		free(fstring);
		data->fstring = new_string;
		data->count = size;
	}
}

void	apply_precision(t_modinfo *modinfo, t_fdata *fdata)
{
	char	spec;
	int		precision;
	char	*fstring;

	if (modinfo == NULL || fdata == NULL || fdata->fstring == NULL)
		return ;
	spec = modinfo->specifier;
	precision = modinfo->precision;
	fstring = fdata->fstring;
	if (precision < 0)
		return ;
	if (spec == 's')
		handle_string_size(fstring, precision, fdata);
	else if (spec == 'd' || spec == 'i' || spec == 'u' || spec == 'x'
		|| spec == 'X' || spec == 'p')
		handle_int_size(fstring, '0', precision, fdata);
}

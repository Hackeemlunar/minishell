/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_nums.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 14:30:23 by hmensah-          #+#    #+#             */
/*   Updated: 2025/02/15 18:00:32 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_fdata	*create_number_data(t_modinfo *info, long nbr, int base)
{
	t_fdata	*data;
	char	*nbr_str;

	nbr_str = ft_itoa_base(nbr, base, 0);
	if (!nbr_str)
		return (NULL);
	if (info->precision == 0 && nbr == 0)
		nbr_str[0] = '\0';
	data = (t_fdata *)malloc(sizeof(t_fdata));
	if (!data)
	{
		free(nbr_str);
		return (NULL);
	}
	data->fstring = nbr_str;
	data->count = ft_strlen(nbr_str);
	return (data);
}

t_fdata	*handle_num_int(t_modinfo *info, va_list args)
{
	int		nbr;
	t_fdata	*data;

	nbr = va_arg(args, int);
	data = create_number_data(info, nbr, 10);
	apply_precision(info, data);
	apply_plus_space_flag(info, data);
	apply_minus_flag(info, data);
	return (data);
}

t_fdata	*handle_num_long(t_modinfo *info, va_list args)
{
	long	nbr;
	t_fdata	*data;

	nbr = va_arg(args, int);
	data = create_number_data(info, nbr, 10);
	apply_precision(info, data);
	apply_plus_space_flag(info, data);
	apply_minus_flag(info, data);
	return (data);
}

static void	apply_hex(t_fdata *data, t_modinfo *info, int upc, char *str)
{
	data->fstring = str;
	data->count = ft_strlen(str);
	apply_precision(info, data);
	if (ft_strchr(info->flags, '#'))
	{
		if (upc)
			apply_prefix("0X", info, data);
		else
			apply_prefix("0x", info, data);
	}
	else
		apply_minus_flag(info, data);
}

t_fdata	*handle_hex(t_modinfo *modinfo, va_list args)
{
	t_fdata			*data;
	unsigned int	nbr;
	char			*nbr_str;
	int				uppercase;

	uppercase = modinfo->specifier == 'X';
	nbr = va_arg(args, int);
	nbr_str = ft_itoa_base(nbr, 16, uppercase);
	if (!nbr_str)
		return (NULL);
	if (modinfo->precision == 0 && nbr == 0)
		nbr_str[0] = '\0';
	data = (t_fdata *)malloc(sizeof(t_fdata));
	if (!data)
	{
		free(nbr_str);
		return (NULL);
	}
	apply_hex(data, modinfo, uppercase, nbr_str);
	return (data);
}

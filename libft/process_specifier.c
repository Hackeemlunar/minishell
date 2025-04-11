/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_specifier.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 14:44:12 by hmensah-          #+#    #+#             */
/*   Updated: 2025/02/15 18:02:42 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

t_fdata	*process_specifier(t_modinfo *modinfo, va_list args)
{
	t_fdata	*data;

	if (modinfo->specifier == 'c')
		data = handle_char(modinfo, args);
	else if (modinfo->specifier == 's')
		data = handle_string(modinfo, args);
	else if (modinfo->specifier == 'p')
		data = handle_pointer(modinfo, args);
	else if (modinfo->specifier == 'u')
		data = handle_uns(modinfo, args);
	else if (modinfo->specifier == 'd')
		data = handle_num_long(modinfo, args);
	else if (modinfo->specifier == 'i')
		data = handle_num_int(modinfo, args);
	else if (modinfo->specifier == 'x' || modinfo->specifier == 'X')
		data = handle_hex(modinfo, args);
	else if (modinfo->specifier == '%')
		data = handle_percent(modinfo);
	else
		return (NULL);
	return (data);
}

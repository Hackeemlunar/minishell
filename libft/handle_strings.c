/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_strings.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 14:42:28 by hmensah-          #+#    #+#             */
/*   Updated: 2025/02/15 18:01:56 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_fdata	*handle_percent(t_modinfo *info)
{
	t_fdata	*data;

	data = (t_fdata *)malloc(sizeof(t_fdata));
	if (!data)
		return (NULL);
	data->count = 1;
	data->fstring = ft_strdup("%");
	apply_minus_flag(info, data);
	return (data);
}

t_fdata	*handle_char(t_modinfo *modinfo, va_list args)
{
	t_fdata	*data;
	char	c;

	data = (t_fdata *)malloc(sizeof(t_fdata));
	if (!data)
		return (NULL);
	c = va_arg(args, int);
	data->count = 1;
	data->fstring = ft_calloc(2, sizeof(char));
	data->fstring[0] = c;
	apply_minus_flag(modinfo, data);
	return (data);
}

t_fdata	*handle_string(t_modinfo *modinfo, va_list args)
{
	t_fdata	*data;
	char	*str;

	data = (t_fdata *)malloc(sizeof(t_fdata));
	if (!data)
		return (NULL);
	str = va_arg(args, char *);
	if (!str)
		str = "(null)";
	data->count = ft_strlen(str);
	data->fstring = ft_strdup(str);
	apply_precision(modinfo, data);
	apply_minus_flag(modinfo, data);
	return (data);
}

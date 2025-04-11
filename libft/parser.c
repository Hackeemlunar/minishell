/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 22:33:50 by hmensah-          #+#    #+#             */
/*   Updated: 2025/02/15 18:02:26 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void	parse_flags(const char *format, t_modinfo *info, int *index)
{
	const char	*valid_flags;
	int			i;

	i = 0;
	valid_flags = "-+ #0";
	while (format[i] && ft_strchr(valid_flags, format[i]))
	{
		if (!ft_strchr(info->flags, format[i]))
			info->flags[ft_strlen(info->flags)] = format[i];
		i++;
	}
	*index += i;
}

static void	parse_width(const char *format, t_modinfo *info, int *index)
{
	int	i;

	i = 0;
	if (ft_isdigit(format[i]))
		info->width = ft_atoi(&format[i]);
	while (ft_isdigit(format[i]))
		i++;
	*index += i;
}

static void	parse_precision(const char *format, t_modinfo *info, int *index)
{
	int	i;

	i = 0;
	if (format[i] == '.')
	{
		i++;
		if (ft_isdigit(format[i]))
			info->precision = ft_atoi(&format[i]);
		else
			info->precision = 0;
		while (ft_isdigit(format[i]))
			i++;
		*index += i;
	}
}

static void	parse_specifier(const char *format, t_modinfo *info)
{
	int	i;

	i = 0;
	if (ft_strchr("cspdiuxX%", format[i]))
		info->specifier = format[i];
}

void	parse_format(const char *format, t_modinfo *info)
{
	int	index;

	index = 0;
	info->width = -1;
	info->precision = -1;
	ft_bzero(info->flags, 5);
	parse_flags(format, info, &index);
	parse_width(format + index, info, &index);
	parse_precision(format + index, info, &index);
	parse_specifier(format + index, info);
	info->flags_count = index + 1;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 18:07:17 by hmensah-          #+#    #+#             */
/*   Updated: 2025/02/15 18:06:27 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	reset_modinfo(t_modinfo *modinfo)
{
	ft_memset(modinfo, 0, sizeof(t_modinfo));
}

static int	h_convsn(const char *format, t_modinfo *info, va_list args, int *i)
{
	t_fdata	*data;
	int		count;

	parse_format(format + *i + 1, info);
	data = process_specifier(info, args);
	*i += info->flags_count;
	reset_modinfo(info);
	ft_putstr_pf(data);
	count = data->count;
	free(data->fstring);
	free(data);
	return (count);
}

int	ft_printf(const char *format, ...)
{
	va_list		args;
	t_modinfo	*info;
	int			cnt;
	int			i;

	cnt = 0;
	i = -1;
	info = malloc(sizeof(t_modinfo));
	if (!info)
		return (-1);
	va_start(args, format);
	while (format[++i])
	{
		if (format[i] == '%')
			cnt += h_convsn(format, info, args, &i);
		else
		{
			ft_putchar_fd(format[i], 1);
			cnt++;
		}
	}
	free(info);
	va_end(args);
	return (cnt);
}

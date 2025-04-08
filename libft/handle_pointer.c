/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pointer.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 14:40:24 by hmensah-          #+#    #+#             */
/*   Updated: 2025/02/15 18:01:44 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	apply_prefix_p(const char *prefix, t_modinfo *info, t_fdata *fdata)
{
	char	*str;
	int		total_width;

	if (info->width > fdata->count + 2)
		total_width = info->width;
	else
		total_width = fdata->count + 2;
	str = ft_calloc(total_width + 1, sizeof(char));
	if (!str)
		return ;
	if (ft_strchr(info->flags, '-'))
	{
		ft_strcpy(str, prefix);
		ft_strlcpy(str + 2, fdata->fstring, fdata->count + 1);
		ft_memset(str + 2 + fdata->count, ' ', total_width - fdata->count - 2);
	}
	else
	{
		ft_memset(str, ' ', total_width - fdata->count - 2);
		ft_strcpy(str + (total_width - fdata->count - 2), prefix);
		ft_strcpy(str + total_width - fdata->count, fdata->fstring);
	}
	free(fdata->fstring);
	fdata->fstring = str;
	fdata->count = total_width;
}

t_fdata	*handle_pointer(t_modinfo *info, va_list args)
{
	t_fdata	*data;
	long	ptr;
	char	*ptr_str;

	ptr = va_arg(args, long);
	ptr_str = ft_itoa_base(ptr, 16, 0);
	if (!ptr_str)
		return (NULL);
	if (info->precision == 0 && ptr == 0)
		ptr_str[0] = '\0';
	data = (t_fdata *)malloc(sizeof(t_fdata));
	if (!data)
	{
		free(ptr_str);
		return (NULL);
	}
	data->fstring = ptr_str;
	data->count = ft_strlen(ptr_str);
	apply_precision(info, data);
	apply_prefix_p("0x", info, data);
	return (data);
}

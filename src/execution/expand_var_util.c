/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var copy.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 14:09:59 by hmensah-          #+#    #+#             */
/*   Updated: 2025/05/15 14:48:35 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	process_variable_build(char **current, char **start,
								char **dest, t_table *table)
{
	int			var_len;
	char		*var_name;
	t_result	value;
	size_t		val_len;

	(*current)++;
	var_len = 0;
	while ((*current)[var_len] && !space_or_quote((*current)[var_len]))
		var_len++;
	var_name = ft_substr(*current, 0, var_len);
	value = get_env(table, var_name);
	if (!value.is_error)
	{
		val_len = ft_strlen(value.data.value);
		ft_memcpy(*dest, value.data.value, val_len);
		*dest += val_len;
	}
	free(var_name);
	*start = *current + var_len;
	*current = *start;
}

void	copy_static_part(char **dest, char **start, char *end)
{
	size_t	part_len;

	part_len = end - *start;
	if (part_len > 0)
	{
		ft_memcpy(*dest, *start, part_len);
		*dest += part_len;
	}
	*start = end;
}

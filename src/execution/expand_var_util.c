/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var_util.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 14:09:59 by hmensah-          #+#    #+#             */
/*   Updated: 2025/05/28 20:32:55 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static void	handle_empty_var(char **current, char **start, char **dest)
{
	**dest = '$';
	(*dest)++;
	*start = *current;
}

static int	get_var_length(char *current)
{
	int	var_len;

	var_len = 0;
	while (current[var_len] && is_valid_var_char(current[var_len]))
		var_len++;
	return (var_len);
}

static void	copy_var_value(char **dest, t_result value)
{
	size_t	val_len;

	if (!value.is_error)
	{
		val_len = ft_strlen(value.data.value);
		ft_memcpy(*dest, value.data.value, val_len);
		*dest += val_len;
	}
}

void	process_variable_build(char **current, char **start,
								char **dest, t_table *table)
{
	int			var_len;
	char		*var_name;
	t_result	value;

	(*current)++;
	var_len = 0;
	if (!(*current)[var_len] || !is_valid_var_char((*current)[var_len]))
	{
		handle_empty_var(current, start, dest);
		return ;
	}
	var_len = get_var_length(*current);
	var_name = ft_substr(*current, 0, var_len);
	value = get_env(table, var_name);
	copy_var_value(dest, value);
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

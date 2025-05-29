/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var_helpers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 00:00:00 by hmensah-          #+#    #+#             */
/*   Updated: 2025/01/27 00:00:00 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	copy_remaining_part(char *dest, char *start, char *current)
{
	size_t	part_len;

	part_len = current - start;
	if (part_len > 0)
	{
		ft_memcpy(dest, start, part_len);
		dest += part_len;
	}
	*dest = '\0';
}

void	process_escape_sequence(char **current, char **dest, char **start)
{
	copy_static_part(dest, start, *current);
	(*current)++;
	**dest = **current;
	(*dest)++;
	(*current)++;
	*start = *current;
}

void	build_expanded_string(char *dest, char *str, t_table *table)
{
	char	*current;
	char	*start;

	current = str;
	start = current;
	while (*current)
	{
		if (*current == '\\' && *(current + 1) == '$')
			process_escape_sequence(&current, &dest, &start);
		else if (*current == '$')
		{
			copy_static_part(&dest, &start, current);
			process_variable_build(&current, &start, &dest, table);
		}
		else
			current++;
	}
	copy_remaining_part(dest, start, current);
}

void	process_variable_length(char **current, char **start,
								size_t *total_len, t_table *table)
{
	int			var_len;
	char		*var_name;
	t_result	value;

	*total_len += *current - *start;
	(*current)++;
	if (!(**current) || !is_valid_var_char(**current))
	{
		(*total_len)++;
		*start = *current;
		return ;
	}
	var_len = 0;
	while ((*current)[var_len] && is_valid_var_char((*current)[var_len]))
		var_len++;
	var_name = ft_substr(*current, 0, var_len);
	value = get_env(table, var_name);
	if (!value.is_error)
		*total_len += ft_strlen(value.data.value);
	free(var_name);
	*start = *current + var_len;
	*current = *start;
}

size_t	calculate_total_length(char *current, t_table *table)
{
	size_t	total_len;
	char	*start;

	total_len = 0;
	start = current;
	while (*current)
	{
		if (*current == '\\' && *(current + 1) == '$')
		{
			total_len += current - start + 1;
			current += 2;
			start = current;
		}
		else if (*current == '$')
			process_variable_length(&current, &start, &total_len, table);
		else
			current++;
	}
	total_len += current - start;
	return (total_len);
}

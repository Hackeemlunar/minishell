/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 14:09:59 by hmensah-          #+#    #+#             */
/*   Updated: 2025/05/24 16:54:47 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	process_variable_build(char **current, char **start,
			char **dest, t_table *table);
void	copy_static_part(char **dest, char **start, char *end);

static void	copy_remaining_part(char *dest, char *start, char *current)
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

static void	build_expanded_string(char *dest, char *str, t_table *table)
{
	char	*current;
	char	*start;

	current = str;
	start = current;
	while (*current)
	{
		if (*current == '$')
		{
			copy_static_part(&dest, &start, current);
			process_variable_build(&current, &start, &dest, table);
		}
		else
		{
			current++;
		}
	}
	copy_remaining_part(dest, start, current);
}

static void	process_variable_length(char **current, char **start,
									size_t *total_len, t_table *table)
{
	int			var_len;
	char		*var_name;
	t_result	value;

	*total_len += *current - *start;
	(*current)++;
	
	// Handle bare $ character (with no variable name following)
	if (!(**current) || space_or_quote(**current))
	{
		(*total_len)++;  // Count the $ character
		*start = *current;
		return;
	}
	
	var_len = 0;
	while ((*current)[var_len] && !space_or_quote((*current)[var_len]))
		var_len++;
	var_name = ft_substr(*current, 0, var_len);
	value = get_env(table, var_name);
	if (!value.is_error)
		*total_len += ft_strlen(value.data.value);
	free(var_name);
	*start = *current + var_len;
	*current = *start;
}

static size_t	calculate_total_length(char *current, t_table *table)
{
	size_t	total_len;
	char	*start;

	total_len = 0;
	start = current;
	while (*current)
	{
		if (*current == '$')
			process_variable_length(&current, &start, &total_len, table);
		else
			current++;
	}
	total_len += current - start;
	return (total_len);
}

char	*expand_variable(char *str, t_allocs *allocs, t_table *table)
{
	char	*expanded;
	char	*current;
	size_t	total_len;
	// int     skip_first;

	// skip_first = 0;
	if (str[0] == '"')
	{
		str++;
		// skip_first = 1;
	}
	current = str;
	total_len = calculate_total_length(current, table);
	expanded = arena_alloc(allocs->exec_alloc, total_len + 1);
	if (!expanded)
		return (NULL);
	build_expanded_string(expanded, str, table);
	return (expanded);
}

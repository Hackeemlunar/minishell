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

char	*expand_variable(char *str, t_allocs *allocs, t_table *table)
{
	char	*expanded;
	char	*current;
	size_t	total_len;

	if (str[0] == '"')
		current = handle_quoted_string(str, allocs);
	else
		current = str;
	if (!current)
		return (NULL);
	total_len = calculate_total_length(current, table);
	expanded = arena_alloc(allocs->exec_alloc, total_len + 1);
	if (!expanded)
		return (NULL);
	build_expanded_string(expanded, current, table);
	return (expanded);
}

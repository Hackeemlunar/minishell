/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 17:23:01 by hmensah-          #+#    #+#             */
/*   Updated: 2025/05/28 19:07:13 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	init_allocators(t_allocs *allocs)
{
	allocs->parse_alloc = arena_create(8192);
	allocs->sh_alloc = arena_create(8192);
	allocs->exec_alloc = arena_create(65536);
}

void	clean_mshell(t_allocs *allocs, t_table *table)
{
	clean_env(table);
	arena_destroy(allocs->parse_alloc);
	arena_destroy(allocs->sh_alloc);
	arena_destroy(allocs->exec_alloc);
}

int	check_all_white_space(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!whitespace(str[i]))
			return (0);
		i++;
	}
	return (1);
}

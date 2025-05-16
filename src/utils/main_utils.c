/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 17:23:01 by hmensah-          #+#    #+#             */
/*   Updated: 2025/05/16 17:30:37 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	init_allocators(t_allocs *allocs)
{
	allocs->parse_alloc = arena_create(8192);
	allocs->sh_alloc = arena_create(8192);
	allocs->exec_alloc = arena_create(8192);
}

void	clean_mshell(t_allocs *allocs, t_table *table)
{
	clean_env(table);
	arena_destroy(allocs->parse_alloc);
	arena_destroy(allocs->sh_alloc);
	arena_destroy(allocs->exec_alloc);
}

t_result	get_paths(t_table *table, char ***paths, t_allocs *allocs)
{
	t_result	result;
	char		*path;
	char		**path_sp;
	int			i;

	result = get_env(table, "PATH");
	if (result.is_error)
		return (result);
	path = result.data.value;
	path_sp = ft_split(path, ':');
	if (!path_sp)
		return (create_error(NO_MEMORY));
	i = 0;
	while (path_sp[i])
		i++;
	*paths = arena_alloc(allocs->sh_alloc, (i + 1) * sizeof(char *));
	i = -1;
	while (path_sp[++i])
	{
		(*paths)[i] = arena_alloc(allocs->sh_alloc, ft_strlen(path_sp[i]) + 3);
		ft_strlcpy((*paths)[i], path_sp[i], ft_strlen(path_sp[i]) + 1);
		ft_strlcat((*paths)[i], "/", ft_strlen(path_sp[i]) + 2);
		free(path_sp[i]);
	}
	(*paths)[i] = NULL;
	return (free(path_sp), create_success(NULL));
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

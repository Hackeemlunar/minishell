/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils_path.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 14:06:52 by hmensah-          #+#    #+#             */
/*   Updated: 2025/05/28 20:18:21 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static int	count_words(char **path_sp)
{
	int	i;

	i = 0;
	while (path_sp[i])
		i++;
	return (i);
}

t_result	get_paths(t_table *table, t_allocs *allocs)
{
	t_result	result;
	char		*path;
	char		**paths;
	char		**path_sp;
	int			i;

	result = get_env(table, "PATH");
	if (result.is_error)
		return (result);
	path = result.data.value;
	path_sp = ft_split(path, ':');
	if (!path_sp)
		return (create_error(NO_MEMORY));
	i = count_words(path_sp);
	paths = arena_alloc(allocs->sh_alloc, (i + 1) * sizeof(char *));
	i = -1;
	while (path_sp[++i])
	{
		paths[i] = arena_alloc(allocs->sh_alloc, ft_strlen(path_sp[i]) + 3);
		ft_strlcpy(paths[i], path_sp[i], ft_strlen(path_sp[i]) + 1);
		ft_strlcat(paths[i], "/", ft_strlen(path_sp[i]) + 2);
		free(path_sp[i]);
	}
	paths[i] = NULL;
	return (free(path_sp), create_success(paths));
}

static inline void	copy_and_concatenate(char *full_path, char **paths,
		int idx, char **argv)
{
	ft_strlcpy(full_path, paths[idx], sizeof(full_path));
	ft_strlcat(full_path, "/", sizeof(full_path));
	ft_strlcat(full_path, argv[0], sizeof(full_path));
}

void	add_full_path(char **argv, t_allocs *allocs, t_table *table)
{
	char		full_path[512];
	int			idx;
	char		**paths;
	t_result	result;

	if (!argv || !argv[0])
		return ;
	result = get_paths(table, allocs);
	if (result.is_error)
		return ;
	if (argv[0][0] == '/' && access(argv[0], F_OK) == 0)
		return ;
	paths = result.data.value;
	ft_memset(full_path, 0, sizeof(full_path));
	idx = -1;
	while (paths[++idx])
	{
		copy_and_concatenate(full_path, paths, idx, argv);
		if (access(full_path, F_OK) == 0)
		{
			argv[0] = arena_alloc(allocs->exec_alloc, ft_strlen(full_path) + 1);
			ft_strlcpy(argv[0], full_path, ft_strlen(full_path) + 1);
			return ;
		}
	}
}

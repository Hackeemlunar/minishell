/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob_processing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 20:34:13 by hmensah-          #+#    #+#             */
/*   Updated: 2025/05/29 20:40:28 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static int	process_directory_entry(struct dirent *entry, char *pattern,
	char ***matches, t_allocs *allocs)
{
	int	count;

	count = 0;
	if (ft_strcmp(entry->d_name, ".") == 0
		|| ft_strcmp(entry->d_name, "..") == 0)
		return (0);
	if (match_pattern(pattern, entry->d_name))
		count = add_match_to_array(matches, entry, allocs, count);
	return (count);
}

static int	initialize_matches(char ***matches, t_allocs *allocs)
{
	*matches = arena_alloc(allocs->exec_alloc, sizeof(char *));
	if (!*matches)
		return (0);
	(*matches)[0] = NULL;
	return (1);
}

int	process_directory_entries(DIR *dir, char *pattern, char ***matches,
	t_allocs *allocs)
{
	struct dirent	*entry;
	int				count;
	int				entry_count;

	if (!initialize_matches(matches, allocs))
		return (0);
	count = 0;
	entry = readdir(dir);
	while (entry != NULL)
	{
		entry_count = process_directory_entry(entry, pattern, matches, allocs);
		if (entry_count > 0)
			count = entry_count;
		entry = readdir(dir);
	}
	return (count);
}

void	copy_args_before_wildcard(char **new_argv, char **old_argv, int idx)
{
	int	i;

	i = 0;
	while (i < idx)
	{
		new_argv[i] = old_argv[i];
		i++;
	}
}

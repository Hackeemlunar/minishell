/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 14:09:59 by hmensah-          #+#    #+#             */
/*   Updated: 2025/05/29 14:09:59 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static char	**allocate_match_array(t_allocs *allocs, int count)
{
	char	**tmp;

	tmp = arena_alloc(allocs->exec_alloc, (count + 2) * sizeof(char *));
	return (tmp);
}

static char	*copy_entry_name(struct dirent *entry, t_allocs *allocs)
{
	char	*name;

	name = arena_alloc(allocs->exec_alloc, ft_strlen(entry->d_name) + 1);
	if (!name)
		return (NULL);
	ft_strlcpy(name, entry->d_name, ft_strlen(entry->d_name) + 1);
	return (name);
}

int	add_match_to_array(char ***matches, struct dirent *entry,
							t_allocs *allocs, int count)
{
	char	**tmp;
	char	**new_matches;

	new_matches = *matches;
	tmp = allocate_match_array(allocs, count);
	if (!tmp)
		return (count);
	if (count > 0)
		ft_memcpy(tmp, new_matches, count * sizeof(char *));
	tmp[count] = copy_entry_name(entry, allocs);
	if (!tmp[count])
		return (count);
	tmp[count + 1] = NULL;
	*matches = tmp;
	return (count + 1);
}

static int	process_wildcard_match(const char **pattern, const char **str,
	const char **backtrack_p, const char **backtrack_s)
{
	if (**pattern == '*')
	{
		*backtrack_p = (*pattern)++;
		*backtrack_s = *str;
		return (1);
	}
	else if (**pattern == **str)
	{
		(*pattern)++;
		(*str)++;
		return (1);
	}
	else if (*backtrack_p)
	{
		*pattern = *backtrack_p + 1;
		*str = ++(*backtrack_s);
		return (1);
	}
	return (0);
}

int	match_pattern(const char *pattern, const char *str)
{
	const char	*backtrack_p;
	const char	*backtrack_s;

	backtrack_p = NULL;
	backtrack_s = NULL;
	while (*str)
	{
		if (!process_wildcard_match(&pattern, &str, &backtrack_p,
				&backtrack_s))
			return (0);
	}
	while (*pattern == '*')
		pattern++;
	return (*pattern == '\0');
}

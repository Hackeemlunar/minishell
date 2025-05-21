/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sngantch <sngantch@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 12:00:00 by sngantch          #+#    #+#             */
/*   Updated: 2025/05/21 12:00:00 by sngantch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include <dirent.h>

static int	match_pattern(const char *pattern, const char *str)
{
	const char	*backtrack_p;
	const char	*backtrack_s;

	backtrack_p = NULL;
	backtrack_s = NULL;
	while (*str)
	{
		if (*pattern == '*')
		{
			backtrack_p = pattern++;
			backtrack_s = str;
		}
		else if (*pattern == *str)
		{
			pattern++;
			str++;
		}
		else if (backtrack_p)
		{
			pattern = backtrack_p + 1;
			str = ++backtrack_s;
		}
		else
			return (0);
	}
	while (*pattern == '*')
		pattern++;
	return (*pattern == '\0');
}

static int	expand_wildcard(char *pattern, char ***matches, t_allocs *allocs)
{
	DIR				*dir;
	struct dirent	*entry;
	int				count;
	char			**new_matches;
	char			**tmp;
	
	dir = opendir(".");
	if (!dir)
		return (0);
	*matches = NULL;
	count = 0;
	new_matches = arena_alloc(allocs->exec_alloc, sizeof(char *));
	if (!new_matches)
	{
		closedir(dir);
		return (0);
	}
	new_matches[0] = NULL;
	while ((entry = readdir(dir)) != NULL)
	{
		if (ft_strcmp(entry->d_name, ".") == 0 || ft_strcmp(entry->d_name, "..") == 0)
			continue;
		
		if (match_pattern(pattern, entry->d_name))
		{
			tmp = arena_alloc(allocs->exec_alloc, (count + 2) * sizeof(char *));
			if (!tmp)
			{
				closedir(dir);
				return (count);
			}
			if (count > 0)
				ft_memcpy(tmp, new_matches, count * sizeof(char *));
			
			tmp[count] = arena_alloc(allocs->exec_alloc, ft_strlen(entry->d_name) + 1);
			if (!tmp[count])
			{
				closedir(dir);
				return (count);
			}
			ft_strlcpy(tmp[count], entry->d_name, ft_strlen(entry->d_name) + 1);
			tmp[count + 1] = NULL;
			new_matches = tmp;
			count++;
		}
	}
	closedir(dir);
	if (count == 0)
	{
		new_matches[0] = pattern;
		*matches = new_matches;
		return (1);
	}
	*matches = new_matches;
	return (count);
}

static int	replace_wildcard(t_ast *ast, int idx, t_allocs *allocs)
{
	char	**matches;
	int		match_count;
	char	**new_argv;
	int		i;
	int		j;
	int		new_size;
	
	match_count = expand_wildcard(ast->data.cmd_node.argv[idx], &matches, allocs);
	if (match_count == 0)
		return (1);
	new_size = ast->data.cmd_node.argc - 1 + match_count;
	new_argv = arena_alloc(allocs->exec_alloc, (new_size + 1) * sizeof(char *));
	if (!new_argv)
		return (0);
	i = 0;
	while (i < idx)
	{
		new_argv[i] = ast->data.cmd_node.argv[i];
		i++;
	}
	j = 0;
	while (j < match_count)
	{
		new_argv[i + j] = matches[j];
		j++;
	}
	i = idx + 1;
	while (i < ast->data.cmd_node.argc)
	{
		new_argv[i - 1 + match_count] = ast->data.cmd_node.argv[i];
		i++;
	}
	new_argv[new_size] = NULL;
	ast->data.cmd_node.argv = new_argv;
	ast->data.cmd_node.argc = new_size;
	return (1);
}

void	expand_wildcards(t_ast *ast, t_allocs *allocs)
{
	int	i;
	int	processed_args;

	if (!ast || ast->type != NODE_CMD || !ast->data.cmd_node.argv)
		return;
	
	i = 0;
	processed_args = 0;
	while (i < ast->data.cmd_node.argc && ast->data.cmd_node.argv[i] && processed_args < 1000)
	{
		if (ast->data.cmd_node.argv[i] && ft_strchr(ast->data.cmd_node.argv[i], '*'))
		{
			if (!replace_wildcard(ast, i, allocs))
				return ;
			i++;
		}
		else
			i++;
		processed_args++;
	}
	if (ast->data.cmd_node.argv && ast->data.cmd_node.argc > 0)
		ast->data.cmd_node.argv[ast->data.cmd_node.argc] = NULL;
}

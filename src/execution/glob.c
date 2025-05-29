/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 12:00:00 by sngantch          #+#    #+#             */
/*   Updated: 2025/05/28 21:05:24 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static int	expand_wildcard(char *pattern, char ***matches, t_allocs *allocs)
{
	DIR	*dir;
	int	count;

	dir = opendir(".");
	if (!dir)
		return (0);
	count = process_directory_entries(dir, pattern, matches, allocs);
	closedir(dir);
	if (count == 0)
	{
		*matches = arena_alloc(allocs->exec_alloc, 2 * sizeof(char *));
		if (!*matches)
			return (0);
		(*matches)[0] = pattern;
		(*matches)[1] = NULL;
		return (1);
	}
	return (count);
}

static int	replace_wildcard(t_ast *ast, int idx, t_allocs *allocs)
{
	char	**matches;
	int		match_cnt;
	char	**new_argv;
	int		new_size;

	match_cnt = expand_wildcard(ast->data.cmd_node.argv[idx], &matches, allocs);
	if (match_cnt == 0)
		return (1);
	new_size = ast->data.cmd_node.argc - 1 + match_cnt;
	new_argv = arena_alloc(allocs->exec_alloc, (new_size + 1) * sizeof(char *));
	if (!new_argv)
		return (0);
	copy_args_before_wildcard(new_argv, ast->data.cmd_node.argv, idx);
	copy_matches_to_argv(new_argv, matches, idx, match_cnt);
	copy_args_after_wildcard(new_argv, ast->data.cmd_node.argv, idx, match_cnt);
	new_argv[new_size] = NULL;
	ast->data.cmd_node.argv = new_argv;
	ast->data.cmd_node.argc = new_size;
	return (1);
}

void	expand_wildcards(t_ast *ast, t_allocs *allocs)
{
	int	i;

	if (!ast || ast->type != NODE_CMD || !ast->data.cmd_node.argv)
		return ;
	i = 0;
	while (i < ast->data.cmd_node.argc && ast->data.cmd_node.argv[i])
	{
		if (ast->data.cmd_node.argv[i]
			&& ft_strchr(ast->data.cmd_node.argv[i], '*'))
		{
			if (!replace_wildcard(ast, i, allocs))
				return ;
		}
		i++;
	}
	if (ast->data.cmd_node.argv && ast->data.cmd_node.argc > 0)
		ast->data.cmd_node.argv[ast->data.cmd_node.argc] = NULL;
}

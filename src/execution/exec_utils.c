/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 14:06:52 by hmensah-          #+#    #+#             */
/*   Updated: 2025/05/15 14:07:11 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	add_full_path(char **argv, char **paths, t_allocs *allocs)
{
	char	full_path[512];
	int		idx;

	if (!paths || !*paths)
		return ;
	if (argv[0][0] == '/' && access(argv[0], F_OK) == 0)
		return ;
	ft_memset(full_path, 0, sizeof(full_path));
	idx = 0;
	while (paths[idx])
	{
		ft_strlcpy(full_path, paths[idx], sizeof(full_path));
		ft_strlcat(full_path, "/", sizeof(full_path));
		ft_strlcat(full_path, argv[0], sizeof(full_path));
		if (access(full_path, F_OK) == 0)
		{
			argv[0] = arena_alloc(allocs->exec_alloc, ft_strlen(full_path) + 1);
			ft_strlcpy(argv[0], full_path, ft_strlen(full_path) + 1);
			return ;
		}
		idx++;
	}
}

int	space_or_quote(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\'' || c == '"');
}

void	remove_leading_quote(t_ast *ast)
{
	int	idx;

	idx = 0;
	while (idx < ast->data.cmd_node.argc)
	{
		if (ast->data.cmd_node.argv[idx][0] == '"'
			|| ast->data.cmd_node.argv[idx][0] == '\'')
			ast->data.cmd_node.argv[idx] = ast->data.cmd_node.argv[idx] + 1;
		idx++;
	}
}

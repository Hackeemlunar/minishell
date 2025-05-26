/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 14:06:52 by hmensah-          #+#    #+#             */
/*   Updated: 2025/05/26 20:49:33 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"


t_result	get_paths(t_table *table, t_allocs *allocs)
{
	t_result	result;
	char		*path;
	char 		**paths;
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

void	add_full_path(char **argv, t_allocs *allocs, t_table *table)
{
	char	full_path[512];
	int		idx;
	char	**paths;
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

int	is_valid_var_char(char c)
{
	return (ft_isalnum(c) || c == '_' || c == '?');
}

void	remove_leading_quote(t_ast *ast)
{
	int	idx;
	int	len;

	idx = 0;
	while (idx < ast->data.cmd_node.argc)
	{
		if (ast->data.cmd_node.argv[idx][0] == '"'
			|| ast->data.cmd_node.argv[idx][0] == '\'')
		{
			ast->data.cmd_node.argv[idx] = ast->data.cmd_node.argv[idx] + 1;
			len = ft_strlen(ast->data.cmd_node.argv[idx]);
			if (len > 0 && (ast->data.cmd_node.argv[idx][len - 1] == '"' 
				|| ast->data.cmd_node.argv[idx][len - 1] == '\''))
			{
				ast->data.cmd_node.argv[idx][len - 1] = '\0';
			}
		}
		idx++;
	}
}

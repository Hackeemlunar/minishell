/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 20:57:40 by sngantch          #+#    #+#             */
/*   Updated: 2025/05/24 17:03:21 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int	handle_builtins(t_ast *node, t_mshell *sh, t_table *table, t_allocs *alloc)
{
	char	**argv;

	argv = node->data.cmd_node.argv;
	if (!argv || !argv[0] || !sh || !table)
		return (1);
	if (ft_strcmp(argv[0], "echo") == 0)
		sh->exit_status = echo(node);
	else if (ft_strcmp(argv[0], "pwd") == 0)
		pwd(&sh->exit_status);
	else if (ft_strcmp(argv[0], "env") == 0)
		env(argv, table);
	else if (ft_strcmp(argv[0], "export") == 0)
		export_command(argv, table, &sh->exit_status);
	else if (ft_strcmp(argv[0], "cd") == 0)
		cd(argv, table, &sh->exit_status);
	else if (ft_strcmp(argv[0], "unset") == 0)
		unset(argv[1], table, &sh->exit_status);
	else if (ft_strcmp(argv[0], "exit") == 0)
		ft_exit(alloc, table);
	else
		return (1);
	return (0);
}

int	is_valid_variable_name(char *key)
{
	int	i;

	i = 0;
	if (!ft_isalpha(key[i]) && key[i] != '_')
		return (0);
	i++;
	while (key[i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

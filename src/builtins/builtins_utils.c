/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sngantch <sngantch@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 20:57:40 by sngantch          #+#    #+#             */
/*   Updated: 2025/05/16 21:23:01 by sngantch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int	handle_builtins(char **argv, t_mshell *sh, t_table *table, t_allocs *alloc)
{
	if (!argv || !argv[0] || !sh || !table)
		return (1);
	if (ft_strcmp(argv[0], "echo") == 0)
		echo(argv);
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

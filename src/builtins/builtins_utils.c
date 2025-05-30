/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 20:57:40 by sngantch          #+#    #+#             */
/*   Updated: 2025/05/30 16:17:10 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int	setup_builtin_redirections(t_in_out *io, t_allocs *alloc,
		t_table *table, int *fds);

static void	restore_fds(int *fds)
{
	dup2(fds[0], STDOUT_FILENO);
	dup2(fds[1], STDIN_FILENO);
	close(fds[0]);
	close(fds[1]);
}

static int	execute_builtin_cmd(char **argv, t_ast *node, t_mshell *sh,
	t_table *table)
{
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
	else if (ft_strcmp(argv[0], "history") == 0)
		history(&sh->exit_status);
	else
		return (1);
	return (0);
}

static int	is_builtin_command(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	else if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	else if (ft_strcmp(cmd, "env") == 0)
		return (1);
	else if (ft_strcmp(cmd, "export") == 0)
		return (1);
	else if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	else if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	else if (ft_strcmp(cmd, "history") == 0)
		return (1);
	else if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

int	handle_builtins(t_ast *node, t_mshell *sh, t_table *table, t_allocs *alloc)
{
	char		**argv;
	t_in_out	*io;
	int			fds[2];
	int			result;

	argv = node->data.cmd_node.argv;
	if (!argv || !argv[0] || !sh || !table)
		return (1);
	if (!is_builtin_command(argv[0]))
		return (1);
	io = node->data.cmd_node.io;
	if (setup_builtin_redirections(io, alloc, table, fds))
		return (1);
	if (ft_strcmp(argv[0], "exit") == 0)
	{
		ft_exit(alloc, table, node, sh);
		restore_fds(fds);
		return (0);
	}
	result = execute_builtin_cmd(argv, node, sh, table);
	if (result)
		return (restore_fds(fds), 1);
	restore_fds(fds);
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

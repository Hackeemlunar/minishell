/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_simple_cmd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 14:38:27 by hmensah-          #+#    #+#             */
/*   Updated: 2025/05/15 15:48:41 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static void	run_child_cmd(t_ast *ast, t_mshell *shell, t_allocs *allocs,
		t_table *table)
{
	t_in_out	*io;

	io = ast->data.cmd_node.io;
	if (io)
	{
		if (set_in_fds(io))
			exit(1);
		if (set_out_fds(io))
			exit(1);
	}
	expand_substitutions(ast, allocs, table);
	remove_leading_quote(ast);
	add_full_path(ast->data.cmd_node.argv, shell->paths, allocs);
	execve(ast->data.cmd_node.argv[0], ast->data.cmd_node.argv, shell->env);
	if (errno == ENOENT)
	{
		ft_putstr_fd(ast->data.cmd_node.argv[0], 2);
		ft_putendl_fd(": command not found", 2);
		exit(127);
	}
	perror("execve");
	exit(126);
}

int	run_simple_cmd(t_ast *ast, t_mshell *shell, t_allocs *allocs,
		t_table *table)
{
	pid_t	pid;
	int		status;

	if (!ast)
		return (perror("pipe"), 1);
	pid = fork();
	if (pid < 0)
		return (perror("fork"), 1);
	if (pid == 0)
		run_child_cmd(ast, shell, allocs, table);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		set_exit_status(shell, WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		set_exit_status(shell, 128 + WTERMSIG(status));
	else if (status != 0)
		set_exit_status(shell, 1);
	return (status);
}

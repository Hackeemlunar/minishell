/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_simple_cmd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sngantch <sngantch@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 14:38:27 by hmensah-          #+#    #+#             */
/*   Updated: 2025/05/25 21:26:24 by sngantch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static void	run_child_cmd(t_ast *ast, t_mshell *shell, t_allocs *allocs, t_table *table)
{
	t_in_out	*io;

	set_signal_handler(ast);
	io = ast->data.cmd_node.io;
	if (io)
	{
		if (set_in_fds(io, allocs, table))
			exit(1);
		if (set_out_fds(io, allocs, table))
			exit(1);
	}
	
	add_full_path(ast->data.cmd_node.argv, allocs, table);
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
		return (perror("Unknown command"), 1);
	expand_substitutions(ast, allocs, table);
	expand_wildcards(ast, allocs);
	remove_leading_quote(ast);
	if (!handle_builtins(ast, shell, table, allocs))
		return (shell->exit_status);
	g_in_child = 1;
	pid = fork();
	if (pid < 0)
		return (perror("fork"), 1);
	if (pid == 0)
		run_child_cmd(ast, shell, allocs, table);
	waitpid(pid, &status, 0);
	g_in_child = 0;
	if (WIFEXITED(status))
		set_exit_status(shell, WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		set_exit_status(shell, 128 + WTERMSIG(status));
	else if (status != 0)
		set_exit_status(shell, 1);
	return (get_exit_status(shell));
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logical_ops.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 10:00:00 by hmensah-          #+#    #+#             */
/*   Updated: 2025/05/14 21:58:28 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int	handle_and_operation(t_ast *ast, t_mshell *shell, 
						t_allocs *allocs, t_table *table)
{
	int		exit_status;
	t_ast	*left;
	t_ast	*right;

	left = ast->data.bin_op_node.left;
	right = ast->data.bin_op_node.right;
	walk_ast(left, shell, allocs, table);
	exit_status = get_exit_status(shell);
	if (exit_status == 0)
	{
		walk_ast(right, shell, allocs, table);
		exit_status = get_exit_status(shell);
	}
	return (exit_status);
}

int	handle_or_operation(t_ast *ast, t_mshell *shell, 
					t_allocs *allocs, t_table *table)
{
	int		exit_status;
	t_ast	*left;
	t_ast	*right;

	left = ast->data.bin_op_node.left;
	right = ast->data.bin_op_node.right;
	walk_ast(left, shell, allocs, table);
	exit_status = get_exit_status(shell);
	if (exit_status != 0)
	{
		walk_ast(right, shell, allocs, table);
		exit_status = get_exit_status(shell);
	}
	return (exit_status);
}

int	handle_bg(t_ast *ast, t_mshell *shell, t_allocs *allocs, t_table *table)
{
	pid_t	pid;
	t_ast	*sub_ast;

	sub_ast = ast->data.sub;
	pid = fork();
	if (pid < 0)
		return (perror("fork"), 1);
	if (pid == 0)
	{
		walk_ast(sub_ast, shell, allocs, table);
		exit(get_exit_status(shell));
	}
	else
	{
		if (shell->pid_track < 10 && shell->pids)
			shell->pids[shell->pid_track++] = pid;
		ft_putchar_fd('[', 1);
		ft_putnbr_fd(shell->pid_track, 1);
		ft_putstr_fd("] ", 1);
		ft_putnbr_fd(pid, 1);
		ft_putchar_fd('\n', 1);
		set_exit_status(shell, 0);
	}
	return (0);
}

int	handle_subsh(t_ast *ast, t_mshell *shell, t_allocs *allocs, t_table *table)
{
	pid_t	pid;
	int		status;
	t_ast	*sub_ast;

	sub_ast = ast->data.sub;
	pid = fork();
	if (pid < 0)
	{
		ft_putendl_fd("fork: Resource temporarily unavailable", 2);
		return (1);
	}
	if (pid == 0)
	{
		walk_ast(sub_ast, shell, allocs, table);
		exit(get_exit_status(shell));
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			set_exit_status(shell, WEXITSTATUS(status));
		else
			set_exit_status(shell, 1);
	}
	return (get_exit_status(shell));
}


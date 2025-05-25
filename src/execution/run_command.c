/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_command.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sngantch <sngantch@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 14:30:01 by hmensah-          #+#    #+#             */
/*   Updated: 2025/05/25 20:55:19 by sngantch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static inline void	handle_others(t_ast *ast, t_mshell *shell,
	t_allocs *allocs, t_table *table)
{
	int	status;

	if (ast->type == NODE_OR)
	{
		status = handle_or_operation(ast, shell, allocs, table);
		set_exit_status(shell, status);
	}
	else if (ast->type == NODE_BACKGROUND)
		handle_bg(ast, shell, allocs, table);
	else if (ast->type == NODE_SUBSHELL)
		handle_subsh(ast, shell, allocs, table);
	else
	{
		ft_putstr_fd("Unknown AST node type: ", 2);
		ft_putnbr_fd(ast->type, 2);
		ft_putchar_fd('\n', 2);
		set_exit_status(shell, 1);
	}
}

void	walk_ast(t_ast *ast, t_mshell *shell, t_allocs *allocs, t_table *table)
{
	int			status;

	if (!ast)
		return ;
	if (ast->type == NODE_CMD)
	{
		status = run_simple_cmd(ast, shell, allocs, table);
		set_exit_status(shell, status);
	}
	else if (ast->type == NODE_PIPELINE)
	{
		status = handle_pipes(ast, shell, allocs, table);
		set_exit_status(shell, status);
	}
	else if (ast->type == NODE_AND)
	{
		status = handle_and_operation(ast, shell, allocs, table);
		set_exit_status(shell, status);
	}
	else
		handle_others(ast, shell, allocs, table);
}

int	run_command(t_mshell *shell, t_allocs *allocs, t_table *table,
		t_result res)
{
	char		*temp;
	int		exit_status;

	shell->ast = res.data.value;
	exit_status = get_exit_status(shell);
	exit_status = exit_status & 0xFF;
	temp = ft_itoa(exit_status);
	add_env(table, "?", temp);
	free(temp);
	walk_ast(shell->ast, shell, allocs, table);
	arena_reset(allocs->exec_alloc);
	arena_reset(allocs->parse_alloc);
	return (get_exit_status(shell));
}

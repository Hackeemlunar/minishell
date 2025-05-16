/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_command.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 14:30:01 by hmensah-          #+#    #+#             */
/*   Updated: 2025/05/16 16:47:27 by hmensah-         ###   ########.fr       */
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
	t_result	result;
	char		*temp;

	shell->ast = res.data.value;
	result = get_env(table, "PATH");
	if (result.is_error)
		return (1);
	temp = ft_itoa(get_exit_status(shell));
	add_env(table, "?", temp);
	free(temp);
	walk_ast(shell->ast, shell, allocs, table);
	arena_reset(allocs->exec_alloc);
	arena_reset(allocs->parse_alloc);
	return (get_exit_status(shell));
}

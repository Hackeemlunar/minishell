

#include "executor.h"

void	walk_ast(t_ast *ast, t_mshell *shell, t_allocs *allocs, t_table *table)
{
	t_result	result;
	int			status;

	if (!ast)
		return ;
	if (ast->type == NODE_CMD)
	{
		expand_substitutions(ast, allocs, table);
		remove_leading_quote(ast);
		result = run_simple_cmd(ast, shell, allocs);
		update_exit_status(shell, result);
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
	else if (ast->type == NODE_OR)
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

t_result	run_command(t_mshell *shell, t_allocs *allocs, t_table *table)
{
	t_result	result;

	result = get_env(table, "PATH");
	if (result.is_error)
		return (result);
	walk_ast(shell->ast, shell, allocs, table);
	arena_reset(allocs->exec_alloc);
	arena_reset(allocs->parse_alloc);
	return (create_success(shell->ast));
}

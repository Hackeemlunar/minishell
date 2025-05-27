/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 18:05:45 by hmensah-          #+#    #+#             */
/*   Updated: 2025/05/05 18:02:16 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_result	parse_pipeline(t_token **current, t_allocs *allocs)
{
	t_result	left_result;
	t_result	right_result;
	t_ast		*left;
	t_ast		*pipe_node;

	left_result = parse_command(current, allocs);
	if (left_result.is_error)
		return (left_result);
	left = left_result.data.value;
	while (peek_token_type(*current) == TOKEN_PIPE)
	{
		*current = advance_token(*current);
		if (peek_token_type(*current) == TOKEN_EOF)
			return (create_error(INVALID_PIPE));
		right_result = parse_command(current, allocs);
		if (right_result.is_error)
			return (right_result);
		if (((t_ast*)right_result.data.value)->type == NODE_CMD && 
			((t_ast*)right_result.data.value)->data.cmd_node.argc == 0 &&
			!has_redirections((t_ast*)right_result.data.value))
			return (create_error(INVALID_PIPE));
		pipe_node = create_ast_node(NODE_PIPELINE, allocs);
		if (!pipe_node)
			return (create_error(NO_MEMORY));
		pipe_node->data.bin_op_node.left = left;
		pipe_node->data.bin_op_node.right = right_result.data.value;
		left = pipe_node;
	}
	return (create_success(left));
}

t_result	parse_logical_and(t_token **current, t_allocs *allocs)
{
	t_result	left_result;
	t_result	right_result;
	t_ast		*left;
	t_ast		*and_node;

	left_result = parse_pipeline(current, allocs);
	if (left_result.is_error)
		return (left_result);
	left = left_result.data.value;
	while (peek_token_type(*current) == TOKEN_AND)
	{
		if (left->type == NODE_CMD && left->data.cmd_node.argc == 0 &&
			!has_redirections(left))
			return (create_error(INVALID_SYNTAX));
		*current = advance_token(*current);
		right_result = parse_pipeline(current, allocs);
		if (right_result.is_error)
			return (right_result);
		if (((t_ast*)right_result.data.value)->type == NODE_CMD && 
			((t_ast*)right_result.data.value)->data.cmd_node.argc == 0 &&
			!has_redirections((t_ast*)right_result.data.value))
			return (create_error(INVALID_SYNTAX));
		and_node = create_ast_node(NODE_AND, allocs);
		if (!and_node)
			return (create_error(NO_MEMORY));
		and_node->data.bin_op_node.left = left;
		and_node->data.bin_op_node.right = right_result.data.value;
		left = and_node;
	}
	return (create_success(left));
}

t_result	parse_logical_or(t_token **current, t_allocs *allocs)
{
	t_result	left_result;
	t_result	right_result;
	t_ast		*left;
	t_ast		*or_node;

	left_result = parse_logical_and(current, allocs);
	if (left_result.is_error)
		return (left_result);
	left = left_result.data.value;
	while (peek_token_type(*current) == TOKEN_OR)
	{
		if (left->type == NODE_CMD && left->data.cmd_node.argc == 0 &&
			!has_redirections(left))
			return (create_error(INVALID_SYNTAX));
		*current = advance_token(*current);
		right_result = parse_logical_and(current, allocs);
		if (right_result.is_error)
			return (right_result);
		if (((t_ast*)right_result.data.value)->type == NODE_CMD
			&& ((t_ast*)right_result.data.value)->data.cmd_node.argc == 0
			&& !has_redirections((t_ast*)right_result.data.value))
			return (create_error(INVALID_SYNTAX));
		or_node = create_ast_node(NODE_OR, allocs);
		if (!or_node)
			return (create_error(NO_MEMORY));
		or_node->data.bin_op_node.left = left;
		or_node->data.bin_op_node.right = right_result.data.value;
		left = or_node;
	}
	return (create_success(left));
}

t_result	parse_expression(t_token **current, t_allocs *allocs)
{
	t_result	result;
	t_ast		*expr_node;

	result = parse_logical_or(current, allocs);
	if (result.is_error)
		return (result);
	expr_node = result.data.value;
	if (peek_token_type(*current) == TOKEN_BG)
	{
		*current = advance_token(*current);
		expr_node = create_ast_node(NODE_BACKGROUND, allocs);
		if (!expr_node)
			return (create_error(NO_MEMORY));
		expr_node->data.sub = result.data.value;
	}
	return (create_success(expr_node));
}

t_result	parse_cmdln(char *cmdln, t_mshell *shell, t_allocs *allocs)
{
	t_result	lex_result;
	t_token		*current;
	t_result	parse_result;

	lex_result = lex_cmdln(cmdln, allocs);
	if (lex_result.is_error)
		return (lex_result);
	current = (t_token *)lex_result.data.value;
	parse_result = parse_expression(&current, allocs);
	if (parse_result.is_error)
		return (parse_result);
	if (peek_token_type(current) != TOKEN_EOF)
		return (create_error(INVALID_SYNTAX));
	shell->ast = (t_ast *)parse_result.data.value;
	return (create_success(shell->ast));
}

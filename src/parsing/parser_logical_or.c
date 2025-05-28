/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_logical_or.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 00:00:00 by hmensah-          #+#    #+#             */
/*   Updated: 2025/05/29 00:00:00 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static t_result	validate_empty_cmd(t_ast *node)
{
	if (node->type == NODE_CMD && node->data.cmd_node.argc == 0
		&& !has_redirections(node))
		return (create_error(INVALID_SYNTAX));
	return (create_success(NULL));
}

static t_result	create_or_node(t_ast *left, t_ast *right, t_allocs *allocs)
{
	t_ast	*or_node;

	or_node = create_ast_node(NODE_OR, allocs);
	if (!or_node)
		return (create_error(NO_MEMORY));
	or_node->data.bin_op_node.left = left;
	or_node->data.bin_op_node.right = right;
	return (create_success(or_node));
}

static t_result	handle_or_operation(t_ast **left, t_token **current,
	t_allocs *allocs)
{
	t_result	right_result;
	t_result	validation_result;
	t_result	node_result;

	validation_result = validate_empty_cmd(*left);
	if (validation_result.is_error)
		return (validation_result);
	*current = advance_token(*current);
	right_result = parse_logical_and(current, allocs);
	if (right_result.is_error)
		return (right_result);
	validation_result = validate_empty_cmd(right_result.data.value);
	if (validation_result.is_error)
		return (validation_result);
	node_result = create_or_node(*left, right_result.data.value, allocs);
	if (node_result.is_error)
		return (node_result);
	*left = node_result.data.value;
	return (create_success(NULL));
}

t_result	parse_logical_or(t_token **current, t_allocs *allocs)
{
	t_result	left_result;
	t_result	operation_result;
	t_ast		*left;

	left_result = parse_logical_and(current, allocs);
	if (left_result.is_error)
		return (left_result);
	left = left_result.data.value;
	while (peek_token_type(*current) == TOKEN_OR)
	{
		operation_result = handle_or_operation(&left, current, allocs);
		if (operation_result.is_error)
			return (operation_result);
	}
	return (create_success(left));
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 18:05:45 by hmensah-          #+#    #+#             */
/*   Updated: 2025/05/29 00:00:00 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

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

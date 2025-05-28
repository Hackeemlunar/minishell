/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_ast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 00:00:00 by hmensah-          #+#    #+#             */
/*   Updated: 2025/05/29 00:00:00 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_token	*advance_token(t_token *current)
{
	if (current)
		return (current->next);
	return (NULL);
}

t_token_type	peek_token_type(t_token *current)
{
	if (!current)
		return (TOKEN_EOF);
	return (current->type);
}

t_ast	*create_ast_node(t_node_type type, t_allocs *allocs)
{
	t_ast	*node;

	node = (t_ast *)arena_alloc(allocs->parse_alloc, sizeof(t_ast));
	if (!node)
		return (NULL);
	ft_memset(node, 0, sizeof(t_ast));
	node->type = type;
	return (node);
}

bool	has_redirections(t_ast *cmd_node)
{
	t_in_out	*io;

	if (!cmd_node || cmd_node->type != NODE_CMD)
		return (false);
	io = cmd_node->data.cmd_node.io;
	if (!io)
		return (false);
	return (io->in_file || io->out_file || io->heredoc_delim
		|| io->out_redirs);
}

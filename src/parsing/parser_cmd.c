/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 18:02:35 by hmensah-          #+#    #+#             */
/*   Updated: 2025/05/05 18:02:54 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_result	parse_command(t_token **current, t_allocs *allocs)
{
	t_ast			*cmd_node;
	t_result		result;
	t_token_type	type;
	char			**new_argv;

	cmd_node = create_ast_node(NODE_CMD, allocs);
	if (!cmd_node)
		return (create_error(NO_MEMORY));
	cmd_node->data.cmd_node.io = (t_in_out *)
		arena_alloc(allocs->parse_alloc, sizeof(t_in_out));
	if (!cmd_node->data.cmd_node.io)
		return (create_error(NO_MEMORY));
	ft_memset(cmd_node->data.cmd_node.io, 0, sizeof(t_in_out));
	cmd_node->data.cmd_node.io->in_fd = -1;
	cmd_node->data.cmd_node.io->out_fd = -1;
	if (peek_token_type(*current) == TOKEN_LPAREN)
	{
		*current = advance_token(*current);
		result = parse_expression(current, allocs);
		if (result.is_error)
			return (result);
		if (peek_token_type(*current) != TOKEN_RPAREN)
			return (create_error(INVALID_SYNTAX));
		*current = advance_token(*current);
		cmd_node->type = NODE_SUBSHELL;
		cmd_node->data.sub = result.data.value;
		return (create_success(cmd_node));
	}
	cmd_node->data.cmd_node.argc = 0;
	while (*current && peek_token_type(*current) != TOKEN_EOF)
	{
		type = peek_token_type(*current);
		if (type == TOKEN_WORD || type == TOKEN_SUBSTITUTION
			|| type == TOKEN_WILDCARD)
		{
			new_argv = arena_alloc(allocs->parse_alloc,
					(cmd_node->data.cmd_node.argc + 2) * sizeof(char *));
			if (!new_argv)
				return (create_error(NO_MEMORY));
			ft_memcpy(new_argv, cmd_node->data.cmd_node.argv,
				cmd_node->data.cmd_node.argc * sizeof(char *));
			new_argv[cmd_node->data.cmd_node.argc] = (*current)->value;
			new_argv[cmd_node->data.cmd_node.argc + 1] = NULL;
			cmd_node->data.cmd_node.argv = new_argv;
			cmd_node->data.cmd_node.argc++;
			*current = advance_token(*current);
		}
		else if (type == TOKEN_REDIR_IN || type == TOKEN_HEREDOC
			|| type == TOKEN_REDIR_OUT || type == TOKEN_APPEND)
		{
			result = parse_redir(cmd_node, current, allocs);
			if (result.is_error)
				return (result);
		}
		else
			break ;
	}
	return (create_success(cmd_node));
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 00:00:00 by hmensah-          #+#    #+#             */
/*   Updated: 2025/05/29 00:00:00 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_result	init_cmd_node(t_ast **cmd_node, t_allocs *allocs)
{
	*cmd_node = create_ast_node(NODE_CMD, allocs);
	if (!*cmd_node)
		return (create_error(NO_MEMORY));
	(*cmd_node)->data.cmd_node.io = (t_in_out *)
		arena_alloc(allocs->parse_alloc, sizeof(t_in_out));
	if (!(*cmd_node)->data.cmd_node.io)
		return (create_error(NO_MEMORY));
	ft_memset((*cmd_node)->data.cmd_node.io, 0, sizeof(t_in_out));
	(*cmd_node)->data.cmd_node.io->in_fd = -1;
	(*cmd_node)->data.cmd_node.io->out_fd = -1;
	(*cmd_node)->data.cmd_node.argc = 0;
	return (create_success(NULL));
}

t_result	handle_subshell(t_token **current, t_ast *cmd_node,
	t_allocs *allocs)
{
	t_result	result;

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

t_result	add_word_to_argv(t_ast *cmd_node, t_token **current,
	t_allocs *allocs)
{
	char	**new_argv;

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
	return (create_success(NULL));
}

int	is_redirection_token(t_token_type type)
{
	return (type == TOKEN_REDIR_IN || type == TOKEN_HEREDOC
		|| type == TOKEN_REDIR_OUT || type == TOKEN_APPEND);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 18:05:45 by hmensah-          #+#    #+#             */
/*   Updated: 2025/05/02 21:35:28 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static t_result	parse_expression(t_token **current, t_allocs *allocs);
static t_result	parse_logical_or(t_token **current, t_allocs *allocs);
static t_result	parse_logical_and(t_token **current, t_allocs *allocs);
static t_result	parse_pipeline(t_token **current, t_allocs *allocs);
static t_result	parse_command(t_token **current, t_allocs *allocs);
t_result	parse_redir(t_ast *cmd_node, t_token **current, t_allocs *allocs);
static t_token	*advance_token(t_token *current);
static t_token_type	peek_token_type(t_token *current);

static t_token	*advance_token(t_token *current)
{
	if (current)
		return (current->next);
	return (NULL);
}

static t_token_type	peek_token_type(t_token *current)
{
	if (!current)
		return (TOKEN_EOF);
	return (current->type);
}

static t_ast	*create_ast_node(t_node_type type, t_allocs *allocs)
{
	t_ast	*node;

	node = (t_ast *)arena_alloc(allocs->parse_alloc, sizeof(t_ast));
	if (!node)
		return (NULL);
	ft_memset(node, 0, sizeof(t_ast));
	node->type = type;
	return (node);
}

t_result	parse_redir(t_ast *cmd_node, t_token **current, t_allocs *allocs)
{
	t_token		*redirect_tok;
	t_token		*file_tok;
	t_in_out	*io;

	(void)allocs;
	redirect_tok = *current;
	if (!redirect_tok || !(redirect_tok->type == TOKEN_REDIR_IN
			|| redirect_tok->type == TOKEN_HEREDOC || redirect_tok->type
			== TOKEN_REDIR_OUT || redirect_tok->type == TOKEN_APPEND))
		return (create_error(INVALID_REDIRECT));
	file_tok = advance_token(*current);
	if (!file_tok || file_tok->type != TOKEN_WORD)
		return (create_error(INVALID_REDIRECT));
	io = cmd_node->data.cmd_node.io;
	if (redirect_tok->type == TOKEN_REDIR_IN)
	{
		io->in_file = file_tok->value;
		io->in_mode = O_RDONLY;
	}
	else if (redirect_tok->type == TOKEN_HEREDOC)
	{
		io->heredoc_delim = file_tok->value;
		io->in_mode = O_RDONLY;
	}
	else if (redirect_tok->type == TOKEN_REDIR_OUT)
	{
		io->out_file = file_tok->value;
		io->out_mode = O_WRONLY | O_CREAT | O_TRUNC;
	}
	else if (redirect_tok->type == TOKEN_APPEND)
	{
		io->out_file = file_tok->value;
		io->out_mode = O_WRONLY | O_CREAT | O_APPEND;
	}
	*current = advance_token(file_tok);
	return (create_success(NULL));
}

static t_result	parse_command(t_token **current, t_allocs *allocs)
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

static t_result	parse_pipeline(t_token **current, t_allocs *allocs)
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
		right_result = parse_command(current, allocs);
		if (right_result.is_error)
			return (right_result);
		pipe_node = create_ast_node(NODE_PIPELINE, allocs);
		if (!pipe_node)
			return (create_error(NO_MEMORY));
		pipe_node->data.bin_op_node.left = left;
		pipe_node->data.bin_op_node.right = right_result.data.value;
		left = pipe_node;
	}
	return (create_success(left));
}

static t_result	parse_logical_and(t_token **current, t_allocs *allocs)
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
		*current = advance_token(*current);
		right_result = parse_pipeline(current, allocs);
		if (right_result.is_error)
			return (right_result);
		and_node = create_ast_node(NODE_AND, allocs);
		if (!and_node)
			return (create_error(NO_MEMORY));
		and_node->data.bin_op_node.left = left;
		and_node->data.bin_op_node.right = right_result.data.value;
		left = and_node;
	}
	return (create_success(left));
}

static t_result	parse_logical_or(t_token **current, t_allocs *allocs)
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
		*current = advance_token(*current);
		right_result = parse_logical_and(current, allocs);
		if (right_result.is_error)
			return (right_result);
		or_node = create_ast_node(NODE_OR, allocs);
		if (!or_node)
			return (create_error(NO_MEMORY));
		or_node->data.bin_op_node.left = left;
		or_node->data.bin_op_node.right = right_result.data.value;
		left = or_node;
	}
	return (create_success(left));
}

static t_result	parse_expression(t_token **current, t_allocs *allocs)
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

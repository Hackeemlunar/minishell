/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_util.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 18:00:24 by hmensah-          #+#    #+#             */
/*   Updated: 2025/05/05 18:04:21 by hmensah-         ###   ########.fr       */
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

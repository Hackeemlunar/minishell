/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 00:00:00 by hmensah-          #+#    #+#             */
/*   Updated: 2025/05/29 00:00:00 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static int	add_output_redir(t_in_out *io, char *filename, int mode,
	t_allocs *allocs)
{
	t_redir	*new_redir;
	t_redir	*current;

	new_redir = arena_alloc(allocs->parse_alloc, sizeof(t_redir));
	if (!new_redir)
		return (1);
	new_redir->filename = filename;
	new_redir->mode = mode;
	new_redir->fd = -1;
	new_redir->next = NULL;
	if (!io->out_redirs)
	{
		io->out_redirs = new_redir;
		return (0);
	}
	current = io->out_redirs;
	while (current->next)
		current = current->next;
	current->next = new_redir;
	return (0);
}

static void	set_redir_params(t_in_out *io, t_token *redirect_tok,
	t_token *file_tok, t_allocs *allocs)
{
	if (redirect_tok->type == TOKEN_REDIR_IN)
	{
		io->in_file = file_tok->value;
		io->in_mode = 0;
	}
	else if (redirect_tok->type == TOKEN_HEREDOC)
	{
		io->heredoc_delim = file_tok->value;
		io->in_mode = 1;
	}
	else if (redirect_tok->type == TOKEN_REDIR_OUT)
	{
		add_output_redir(io, file_tok->value, 0, allocs);
	}
	else if (redirect_tok->type == TOKEN_APPEND)
	{
		add_output_redir(io, file_tok->value, 1, allocs);
	}
}

t_result	parse_redir(t_ast *cmd_node, t_token **current, t_allocs *allocs)
{
	t_token		*redirect_tok;
	t_token		*file_tok;
	t_in_out	*io;

	redirect_tok = *current;
	if (!redirect_tok || !(redirect_tok->type == TOKEN_REDIR_IN
			|| redirect_tok->type == TOKEN_HEREDOC
			|| redirect_tok->type == TOKEN_REDIR_OUT
			|| redirect_tok->type == TOKEN_APPEND))
		return (create_error(INVALID_REDIRECT));
	file_tok = advance_token(*current);
	if (!file_tok || file_tok->type != TOKEN_WORD)
		return (create_error(INVALID_REDIRECT));
	io = cmd_node->data.cmd_node.io;
	set_redir_params(io, redirect_tok, file_tok, allocs);
	*current = advance_token(file_tok);
	return (create_success(NULL));
}

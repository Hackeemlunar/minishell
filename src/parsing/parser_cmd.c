/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 18:02:35 by hmensah-          #+#    #+#             */
/*   Updated: 2025/05/29 00:00:00 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static t_result	process_token(t_ast *cmd_node, t_token **current,
	t_allocs *allocs)
{
	t_result		result;
	t_token_type	type;

	type = peek_token_type(*current);
	if (type == TOKEN_WORD || type == TOKEN_WILDCARD)
	{
		result = add_word_to_argv(cmd_node, current, allocs);
		if (result.is_error)
			return (result);
	}
	else if (is_redirection_token(type))
	{
		result = parse_redir(cmd_node, current, allocs);
		if (result.is_error)
			return (result);
	}
	else
		return (create_error(INVALID_SYNTAX));
	return (create_success(NULL));
}

t_result	parse_command(t_token **current, t_allocs *allocs)
{
	t_ast		*cmd_node;
	t_result	result;

	result = init_cmd_node(&cmd_node, allocs);
	if (result.is_error)
		return (result);
	if (peek_token_type(*current) == TOKEN_LPAREN)
		return (handle_subshell(current, cmd_node, allocs));
	while (*current && peek_token_type(*current) != TOKEN_EOF)
	{
		result = process_token(cmd_node, current, allocs);
		if (result.is_error)
		{
			if (result.data.error == INVALID_SYNTAX)
				break ;
			return (result);
		}
	}
	return (create_success(cmd_node));
}

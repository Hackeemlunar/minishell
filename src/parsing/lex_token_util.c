/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_token_util.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 18:09:18 by hmensah-          #+#    #+#             */
/*   Updated: 2025/05/05 18:10:48 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_result	handle_unit(t_lexer *lexer, char token)
{
	char			*value;
	t_token_type	token_type;

	lexer->pos++;
	value = (char *)arena_alloc(lexer->alloc, 2);
	if (!value)
		return (create_error(NO_MEMORY));
	value[0] = token;
	value[1] = '\0';
	if (token == '(')
		token_type = TOKEN_LPAREN;
	else if (token == ')')
		token_type = TOKEN_RPAREN;
	else
		token_type = TOKEN_WILDCARD;
	return (new_token(token_type, value, lexer->alloc));
}

t_result	handle_substitution(t_lexer *lexer)
{
	char	*value;

	lexer->pos++;
	value = (char *)arena_alloc(lexer->alloc, 2);
	if (!value)
		return (create_error(NO_MEMORY));
	value[0] = '$';
	value[1] = '\0';
	return (new_token(TOKEN_SUBSTITUTION, value, lexer->alloc));
}

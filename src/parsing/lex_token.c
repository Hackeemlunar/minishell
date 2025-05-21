/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_token.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 18:36:15 by hmensah-          #+#    #+#             */
/*   Updated: 2025/05/05 18:12:05 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

/**
 * handle pipe character
 */
static inline t_result	handle_pipe_or(t_lexer *lexer)
{
	char	*value;

	lexer->pos++;
	if (lexer->pos < lexer->len && lexer->input[lexer->pos] == '|')
	{
		lexer->pos++;
		value = (char *)arena_alloc(lexer->alloc, 3);
		if (!value)
			return (create_error(NO_MEMORY));
		ft_strlcpy(value, "||", 3);
		return (new_token(TOKEN_OR, value, lexer->alloc));
	}
	value = (char *)arena_alloc(lexer->alloc, 2);
	if (!value)
		return (create_error(NO_MEMORY));
	value[0] = '|';
	value[1] = '\0';
	return (new_token(TOKEN_PIPE, value, lexer->alloc));
}

/**
 * Handle in redirection characters
 * - '<' for input redirection
 * - '<<' for here-document
 */
static inline t_result	handle_redir_in(t_lexer *lexer)
{
	char	*value;

	lexer->pos++;
	if (lexer->pos < lexer->len && lexer->input[lexer->pos] == '<')
	{
		lexer->pos++;
		value = (char *)arena_alloc(lexer->alloc, 3);
		if (!value)
			return (create_error(NO_MEMORY));
		ft_strlcpy(value, "<<", 3);
		return (new_token(TOKEN_HEREDOC, value, lexer->alloc));
	}
	value = (char *)arena_alloc(lexer->alloc, 2);
	if (!value)
		return (create_error(NO_MEMORY));
	value[0] = '<';
	value[1] = '\0';
	return (new_token(TOKEN_REDIR_IN, value, lexer->alloc));
}

/**
 * Handle out redirection characters
 * - '>' for output redirection
 * - '>>' for append redirection
 */
static inline t_result	handle_redir_out(t_lexer *lexer)
{
	char	*value;

	lexer->pos++;
	if (lexer->pos < lexer->len && lexer->input[lexer->pos] == '>')
	{
		lexer->pos++;
		value = (char *)arena_alloc(lexer->alloc, 3);
		if (!value)
			return (create_error(NO_MEMORY));
		ft_strlcpy(value, ">>", 3);
		return (new_token(TOKEN_APPEND, value, lexer->alloc));
	}
	value = (char *)arena_alloc(lexer->alloc, 2);
	if (!value)
		return (create_error(NO_MEMORY));
	value[0] = '>';
	value[1] = '\0';
	return (new_token(TOKEN_REDIR_OUT, value, lexer->alloc));
}

static inline t_result	handle_and_bg(t_lexer *lexer)
{
	char	*value;

	lexer->pos++;
	if (lexer->pos < lexer->len && lexer->input[lexer->pos] == '&')
	{
		lexer->pos++;
		value = (char *)arena_alloc(lexer->alloc, 3);
		if (!value)
			return (create_error(NO_MEMORY));
		ft_strlcpy(value, "&&", 3);
		return (new_token(TOKEN_AND, value, lexer->alloc));
	}
	value = (char *)arena_alloc(lexer->alloc, 2);
	if (!value)
		return (create_error(NO_MEMORY));
	value[0] = '&';
	value[1] = '\0';
	return (new_token(TOKEN_BG, value, lexer->alloc));
}

/**
 * Get the next token from the input
 */
t_result	get_next_token(t_lexer *lexer)
{
	t_result	result;

	skip_whitespace(lexer);
	if (lexer->pos >= lexer->len)
		return (new_token(TOKEN_EOF, NULL, lexer->alloc));
	if (lexer->input[lexer->pos] == '|')
		return (handle_pipe_or(lexer));
	if (lexer->input[lexer->pos] == '<')
		return (handle_redir_in(lexer));
	if (lexer->input[lexer->pos] == '>')
		return (handle_redir_out(lexer));
	if (lexer->input[lexer->pos] == '$')
		return (handle_substitution(lexer));
	if (lexer->input[lexer->pos] == '&')
		return (handle_and_bg(lexer));
	if (lexer->input[lexer->pos] == '(' || lexer->input[lexer->pos] == ')')
		return (handle_unit(lexer, lexer->input[lexer->pos]));
	result = extract_word(lexer);
	if (result.is_error)
		return (result);
	return (new_token(TOKEN_WORD, result.data.value, lexer->alloc));
}

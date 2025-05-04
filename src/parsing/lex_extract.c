/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_extract.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 19:21:06 by hmensah-          #+#    #+#             */
/*   Updated: 2025/05/04 19:23:04 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

/**
 * Handle quoted strings in lexing process
 * Following bash behavior:
 * - Single quotes preserve literal value of characters
 * - Double quotes allow parameter and command substitution
 * - We copy the leading quote to determine if we are in a 
 * - single or double quote. This helps handle substitution
 */
static t_result	handle_quotes(t_lexer *lexer, char quote_type)
{
	int		start;
	int		quoted_len;
	char	*result;

	if (!lexer || !lexer->alloc)
		return (create_error(ERROR));
	start = lexer->pos++;
	quoted_len = 0;
	while (lexer->pos < lexer->len)
	{
		if (lexer->input[lexer->pos] == quote_type
			&& lexer->input[lexer->pos - 1] != '\\')
			break ;
		lexer->pos++;
		quoted_len++;
	}
	if (lexer->pos >= lexer->len && lexer->input[lexer->pos] != quote_type)
		return (create_error(INVALID_QUOTE));
	quoted_len++;
	lexer->pos++;
	result = (char *)arena_alloc(lexer->alloc, quoted_len + 1);
	if (!result)
		return (create_error(NO_MEMORY));
	quote_strcpy(result, &lexer->input[start], quoted_len + 1);
	return (create_success(result));
}

/**
 * Handle escape sequences in words
 */
static int	handle_escape(t_lexer *lexer, char *result, int pos)
{
	if (lexer->pos + 1 < lexer->len)
	{
		lexer->pos++;
		result[pos] = lexer->input[lexer->pos];
		lexer->pos++;
		return (1);
	}
	else
	{
		result[pos] = '\\';
		lexer->pos++;
	}
	return (0);
}

static int	copy_word(t_lexer *lexer, char *result)
{
	int		i;
	int		quoted;
	char	quote_type;

	i = 0;
	quoted = 0;
	quote_type = 0;
	while (lexer->pos < lexer->len)
	{
		if ((lexer->input[lexer->pos] == '"'
				|| lexer->input[lexer->pos] == '\'') && !quoted)
			quote_type = lexer->input[lexer->pos];
		if (lexer->input[lexer->pos] == quote_type)
			quoted = !quoted;
		if (!quoted && is_special_char(lexer->input[lexer->pos]))
			break ;
		if (lexer->input[lexer->pos] == '\\')
			i += handle_escape(lexer, result, i);
		else
			result[i++] = lexer->input[lexer->pos++];
	}
	if (quoted)
		return (1);
	result[i] = '\0';
	return (0);
}

static int	count_char(t_lexer *lexer)
{
	int		i;
	int		quoted;
	char	quote_type;

	i = 0;
	quoted = 0;
	quote_type = 0;
	while (lexer->pos < lexer->len)
	{
		if ((lexer->input[lexer->pos] == '"'
				|| lexer->input[lexer->pos] == '\'') && !quoted)
			quote_type = lexer->input[lexer->pos];
		if (lexer->input[lexer->pos] == quote_type)
			quoted = !quoted;
		if (!quoted && is_special_char(lexer->input[lexer->pos]))
			break ;
		if (lexer->input[lexer->pos++] == '\\')
			lexer->pos += (++i);
		else
			++i;
	}
	return (i);
}

/**
 * Extract a word token, handling quotes and escapes
 * Handle quotes if present
 * copy word with proper escape handling
 */
t_result	extract_word(t_lexer *lexer)
{
	int		start;
	int		i;
	char	*result;

	start = lexer->pos;
	if (lexer->input[lexer->pos] == '"' || lexer->input[lexer->pos] == '\'')
		return (handle_quotes(lexer, lexer->input[lexer->pos]));
	i = count_char(lexer);
	result = (char *)arena_alloc(lexer->alloc, i + 1);
	if (!result)
		return (create_error(NO_MEMORY));
	lexer->pos = start;
	if (copy_word(lexer, result))
		return (create_error(INVALID_QUOTE));
	return (create_success(result));
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 14:01:06 by hmensah-          #+#    #+#             */
/*   Updated: 2025/05/28 19:10:21 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

/**
 * Initialize a lexer with the given input
 */
static t_result	init_lexer(const char *input, t_arena *alloc)
{
	t_lexer	*lexer;

	if (!input || !alloc)
		return (create_error(ERROR));
	lexer = (t_lexer *)arena_alloc(alloc, sizeof(t_lexer));
	if (!lexer)
		return (create_error(NO_MEMORY));
	lexer->input = input;
	lexer->pos = 0;
	lexer->len = ft_strlen(input);
	lexer->alloc = alloc;
	return (create_success(lexer));
}

/**
 * Skip whitespace characters
 */

void	skip_whitespace(t_lexer *lexer)
{
	while (lexer->pos < lexer->len && (lexer->input[lexer->pos] == ' '
			|| lexer->input[lexer->pos] == '\t'
			|| lexer->input[lexer->pos] == '\n'))
	{
		lexer->pos++;
	}
}

/**
 * Create a new token with the given type and value
 */
t_result	new_token(t_token_type type, char *value, t_arena *alloc)
{
	t_token	*token;

	if (!alloc)
		return (create_error(ERROR));
	token = (t_token *)arena_alloc(alloc, sizeof(t_token));
	if (!token)
		return (create_error(NO_MEMORY));
	token->type = type;
	token->value = value;
	token->next = NULL;
	return (create_success(token));
}

/**
 * quote_strcpy
 * This is to handle excape sequences in quoted strings
*/
size_t	quote_strcpy(char *dst, const char *src, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (j < len)
	{
		if (src[j] == '\\' && j + 1 < len && (src[j + 1] == '"'
				|| src[j + 1] == '\''))
		{
			j++;
			continue ;
		}
		dst[i] = src[j];
		i++;
		j++;
	}
	dst[i] = '\0';
	return (i);
}

/**
 * Tokenize the input command line
 */
t_result	lex_cmdln(const char *cmdline, t_allocs *allocs)
{
	t_result	result;
	t_lexer		*lexer;
	t_token		*head;
	t_token		*current;	

	if (!cmdline || !allocs->parse_alloc)
		return (create_error(ERROR));
	result = init_lexer(cmdline, allocs->parse_alloc);
	if (result.is_error)
		return (result);
	lexer = result.data.value;
	result = get_next_token(lexer);
	if (result.is_error)
		return (result);
	head = result.data.value;
	current = head;
	while (current->type != TOKEN_EOF)
	{
		result = get_next_token(lexer);
		if (result.is_error)
			return (result);
		current->next = result.data.value;
		current = current->next;
	}
	return (create_success(head));
}

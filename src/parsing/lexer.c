/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 14:01:06 by hmensah-          #+#    #+#             */
/*   Updated: 2025/05/01 16:55:28 by hmensah-         ###   ########.fr       */
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

static void	skip_whitespace(t_lexer *lexer)
{
	while (lexer->pos < lexer->len &&
		(lexer->input[lexer->pos] == ' ' ||
		lexer->input[lexer->pos] == '\t' ||
		lexer->input[lexer->pos] == '\n'))
	{
		lexer->pos++;
	}
}

/**
 * Check if character is a special token character
 */
static bool	is_special_char(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == ' '
		|| c == '(' || c == ')' || c == '$' || c == '&' || c == '\t'
		|| c == '\n' || c == '\0');
}

/**
 * Create a new token with the given type and value
 */
static t_result	new_token(t_token_type type, char *value, t_arena *alloc)
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
static size_t quote_strcpy(char *dst, const char *src, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (j < len - 1)
	{
		if (src[j] == '\\' && (src[j + 1] == '"' || src[j + 1] == '\''))
		{
			j++;
			continue;
		}
		dst[i] = src[j];
		i++;
		j++;
	}
	dst[i++] = '\0';
	return (i);
}


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
	char		*result;

	if (!lexer || !lexer->alloc)
		return (create_error(ERROR));
	start = lexer->pos++;
	quoted_len = 0;
	while (lexer->pos < lexer->len)
	{
		if (lexer->input[lexer->pos] == quote_type
			&& lexer->input[lexer->pos - 1] != '\\')
			break;
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

/**
 * Extract a word token, handling quotes and escapes
 * Handle quotes if present
 * copy word with proper escape handling
 */
static t_result	extract_word(t_lexer *lexer)
{
	int		start;
	int		i;
	char		*result;

	start = lexer->pos;
	if (lexer->input[lexer->pos] == '"' || lexer->input[lexer->pos] == '\'')
		return (handle_quotes(lexer, lexer->input[lexer->pos]));
	i = 0;
	while (lexer->pos < lexer->len && !is_special_char(lexer->input[lexer->pos]))
	{
		if (lexer->input[lexer->pos] == '\\')
			lexer->pos += 2;
		else
			lexer->pos++;
		i++;
	}
	result = (char *)arena_alloc(lexer->alloc, i + 1);
	if (!result)
		return (create_error(NO_MEMORY));
	lexer->pos = start;
	i = 0;
	while (lexer->pos < lexer->len && !is_special_char(lexer->input[lexer->pos]))
	{
		if (lexer->input[lexer->pos] == '\\')
			i += handle_escape(lexer, result, i);
		else
		{
			result[i++] = lexer->input[lexer->pos];
			lexer->pos++;
		}
	}
	result[i] = '\0';
	return (create_success(result));
}

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

static inline t_result	handle_substitution(t_lexer *lexer)
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


static inline t_result	handle_unit(t_lexer *lexer, char token)
{

	char		*value;
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

/**
 * Get the next token from the input
 */
static t_result	get_next_token(t_lexer *lexer)
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
	if (lexer->input[lexer->pos] == '(' || lexer->input[lexer->pos] == '*'
		|| lexer->input[lexer->pos] == ')')
		return (handle_unit(lexer, lexer->input[lexer->pos]));
	result = extract_word(lexer);
	if (result.is_error)
		return (result);
	return (new_token(TOKEN_WORD, result.data.value, lexer->alloc));
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

/**
* a main function to test the lexer
*/
/**
int main()
{
	t_allocs	*allocs = malloc(sizeof(t_allocs));
	t_result	result;
	t_token		*token;
	 char		*input = NULL;

	allocs->parse_alloc = arena_create(4096);
	do {
		if (input)
			free(input);
		input = readline("Enter command> ");
		result = lex_cmdln(input, allocs);
		if (result.is_error)
		{
			printf("Error: %d\n", result.data.error);
			return (1);
		}
		token = result.data.value;
		while (token)
		{
			printf("Token: %s\n", token->value);
			token = token->next;
		}
		arena_reset(allocs->parse_alloc);
	} while (input);
	if (input)
		free(input);
	free(allocs);
	arena_destroy(allocs->parse_alloc);
	return (0);
}
*/

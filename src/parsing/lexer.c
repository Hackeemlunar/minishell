
#include "parser.h"

/**
 * Initialize a lexer with the given input
 */

static t_lexer	init_lexer(const char *input, t_arena *alloc)
{
	t_lexer	lexer;

	lexer.input = input;
	lexer.pos = 0;
	lexer.len = ft_strlen(input);
	lexer.alloc = alloc;
	return (lexer);
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
static int	is_special_char(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == ' ' ||
			c == '\t' || c == '\n' || c == '\0');
}

/**
 * Create a new token with the given type and value
 */
static t_token	*new_token(t_token_type type, char *value, t_arena *alloc)
{
	t_token	*token;

	token = (t_token *)arena_alloc(alloc, sizeof(t_token));
	if (!token)
		return (NULL);

	token->type = type;
	token->value = value;
	token->next = NULL;

	return (token);
}

/**
 * Handle quoted strings in lexing process
 * Following bash behavior:
 * - Single quotes preserve literal value of characters
 * - Double quotes allow parameter and command substitution
 */
static char	*handle_quotes(t_lexer *lexer, char quote_type)
{
	int		start;
	int		quoted_len;
	char	*result;

	// Skip the opening quote
	start = ++lexer->pos;
	quoted_len = 0;

	// Find the closing quote
	while (lexer->pos < lexer->len && lexer->input[lexer->pos] != quote_type)
	{
		lexer->pos++;
		quoted_len++;
	}

	// If we found a closing quote, skip it
	if (lexer->pos < lexer->len && lexer->input[lexer->pos] == quote_type)
		lexer->pos++;
	else
		return (NULL); // Unclosed quote is an error

	// Extract the quoted string without the quotes
	result = (char *)arena_alloc(lexer->alloc, quoted_len + 1);
	if (!result)
		return (NULL);

	ft_strlcpy(result, &lexer->input[start], quoted_len + 1);
	return (result);
}

/**
 * Handle escape sequences in words
 */
static int	handle_escape(t_lexer *lexer, char *result, int pos)
{
	if (lexer->pos + 1 < lexer->len)
	{
		lexer->pos++; // Skip the backslash
		result[pos] = lexer->input[lexer->pos];
		lexer->pos++;
		return (1);
	}
	return (0);
}

/**
 * Extract a word token, handling quotes and escapes
 */
static char	*extract_word(t_lexer *lexer)
{
	int		start;
	int		i;
	char	*result;
	char	*quoted;

	start = lexer->pos;

	// Handle quotes if present
	if (lexer->input[lexer->pos] == '"' || lexer->input[lexer->pos] == '\'')
	{
		quoted = handle_quotes(lexer, lexer->input[lexer->pos]);
		return (quoted);
	}

	// Find length of word
	i = 0;
	while (lexer->pos < lexer->len && !is_special_char(lexer->input[lexer->pos]))
	{
		if (lexer->input[lexer->pos] == '\\')
			lexer->pos += 2; // Skip backslash and escaped char
		else
			lexer->pos++;
		i++;
	}

	result = (char *)arena_alloc(lexer->alloc, i + 1);
	if (!result)
		return (NULL);

	// Copy word with proper escape handling
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

	return (result);
}

/**
 * Get the next token from the input
 */
static t_token	*get_next_token(t_lexer *lexer)
{
	char	*value;

	skip_whitespace(lexer);

	if (lexer->pos >= lexer->len)
		return (new_token(TOKEN_EOF, NULL, lexer->alloc));

	// Check for pipe
	if (lexer->input[lexer->pos] == '|')
	{
		lexer->pos++;
		value = (char *)arena_alloc(lexer->alloc, 2);
		if (!value)
			return (NULL);
		value[0] = '|';
		value[1] = '\0';
		return (new_token(TOKEN_PIPE, value, lexer->alloc));
	}

	// Check for redirections
	if (lexer->input[lexer->pos] == '<')
	{
		lexer->pos++;
		if (lexer->pos < lexer->len && lexer->input[lexer->pos] == '<')
		{
			lexer->pos++;
			value = (char *)arena_alloc(lexer->alloc, 3);
			if (!value)
				return (NULL);
			ft_strlcpy(value, "<<", 3);
			return (new_token(TOKEN_HEREDOC, value, lexer->alloc));
		}
		value = (char *)arena_alloc(lexer->alloc, 2);
		if (!value)
			return (NULL);
		value[0] = '<';
		value[1] = '\0';
		return (new_token(TOKEN_REDIR_IN, value, lexer->alloc));
	}

	if (lexer->input[lexer->pos] == '>')
	{
		lexer->pos++;
		if (lexer->pos < lexer->len && lexer->input[lexer->pos] == '>')
		{
			lexer->pos++;
			value = (char *)arena_alloc(lexer->alloc, 3);
			if (!value)
				return (NULL);
			ft_strlcpy(value, ">>", 3);
			return (new_token(TOKEN_APPEND, value, lexer->alloc));
		}
		value = (char *)arena_alloc(lexer->alloc, 2);
		if (!value)
			return (NULL);
		value[0] = '>';
		value[1] = '\0';
		return (new_token(TOKEN_REDIR_OUT, value, lexer->alloc));
	}

	// Must be a word (command, argument, or filename)
	value = extract_word(lexer);
	if (!value)
		return (NULL);

	return (new_token(TOKEN_WORD, value, lexer->alloc));
}

/**
 * Tokenize the input command line
 */
t_result	lex_cmdln(const char *cmdline, t_mshell *shell, t_allocs *allocs)
{
	t_lexer		lexer;
	t_token		*head;
	t_token		*current;
	t_token		*next_token;

	if (!cmdline || !allocs->parse_alloc)
		return (create_error(ERROR));
	lexer = init_lexer(cmdline, allocs->parse_alloc);
	next_token = get_next_token(&lexer);
	if (!next_token)
		return (create_error(NO_MEMORY));
	head = next_token;
	current = head;
	while (current->type != TOKEN_EOF)
	{
		next_token = get_next_token(&lexer);
		if (!next_token)
			return (create_error(NO_MEMORY));
		current->next = next_token;
		current = next_token;
	}
	return (create_success(head));
}

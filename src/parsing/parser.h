#ifndef PARSER_H
# define PARSER_H
# include "../minishell.h"

/**
 * Token types for the lexer
 */
typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
	TOKEN_EOF,
	TOKEN_SUBSTITUTION,
	TOKEN_OR,
	TOKEN_AND,
	TOKEN_WILDCARD,
	TOKEN_BG,
	TOKEN_LPAREN,
	TOKEN_RPAREN,
} t_token_type;

/**
 * Token structure for the lexer
 */
typedef struct s_token
{
	t_token_type type;
	char *value;
	struct s_token *next;
} t_token;

/**
 * Lexer context to track position and state
 */
typedef struct s_lexer
{
	const char	*input;
	int		pos;
	int		len;
	t_arena		*alloc;
} t_lexer;

/* Function prototypes */

#endif

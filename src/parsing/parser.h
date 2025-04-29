#ifndef PARSER_H
# define PARSER_H
# include "../minishell.h"

/**
 * Token types for the lexer
 */
typedef enum e_token_type
{
	TOKEN_WORD,       // Command or argument
	TOKEN_PIPE,       // Pipe character |
	TOKEN_REDIR_IN,   // Input redirection <
	TOKEN_REDIR_OUT,  // Output redirection >
	TOKEN_APPEND,     // Append redirection >>
	TOKEN_HEREDOC,    // Heredoc redirection <<
	TOKEN_EOF         // End of input
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
	int			pos;
	int			len;
	t_arena		*alloc;
} t_lexer;

/* Function prototypes */
t_result	lex_cmdln(const char *cmdline, t_mshell *shell, t_allocs *allocs);
t_result	parse_cmdln(t_result *lex_result, t_mshell *shell, t_allocs *allocs);

#endif
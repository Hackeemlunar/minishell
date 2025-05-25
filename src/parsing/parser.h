/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 18:49:13 by hmensah-          #+#    #+#             */
/*   Updated: 2025/05/05 18:11:55 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
}	t_token_type;

/**
 * Token structure for the lexer
 */
typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}	t_token;

/**
 * Lexer context to track position and state
 */
typedef struct s_lexer
{
	const char	*input;
	int			pos;
	int			len;
	t_arena		*alloc;
}	t_lexer;

/* Function prototypes */
t_result		lex_cmdln(const char *cmdline, t_allocs *allocs);
t_result		extract_word(t_lexer *lexer);
size_t			quote_strcpy(char *dst, const char *src, size_t len);
t_result		get_next_token(t_lexer *lexer);
t_result		new_token(t_token_type type, char *value, t_arena *alloc);
void			skip_whitespace(t_lexer *lexer);
t_result		parse_expression(t_token **current, t_allocs *allocs);
t_result		parse_logical_or(t_token **current, t_allocs *allocs);
t_result		parse_logical_and(t_token **current, t_allocs *allocs);
t_result		parse_pipeline(t_token **current, t_allocs *allocs);
t_result		parse_command(t_token **current, t_allocs *allocs);
t_result		parse_redir(t_ast *node, t_token **current, t_allocs *allocs);
t_token			*advance_token(t_token *current);
t_token_type	peek_token_type(t_token *current);
t_ast			*create_ast_node(t_node_type type, t_allocs *allocs);
bool			has_redirections(t_ast *cmd_node);
t_result		handle_unit(t_lexer *lexer, char token);
t_result		handle_substitution(t_lexer *lexer);

#endif

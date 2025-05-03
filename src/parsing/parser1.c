


#include "parser.h"

// -- Helpers --------------------------------------------------

static t_token_type peek_token_type(t_token *cur) {
  return cur ? cur->type : TOKEN_EOF;
}
static t_token *advance_token(t_token *cur) {
  return cur ? cur->next : NULL;
}
static t_ast *ast_new_node(t_node_type type, t_allocs *a) {
  t_ast *n = arena_alloc(a->parse_alloc, sizeof(*n));
  if (!n) return NULL;
  n->type = type;
  return n;
}

// Forward declares
static t_result parse_expression(t_token **cur, t_allocs *a);
static t_result parse_logical_or(t_token **cur, t_allocs *a);
static t_result parse_logical_and(t_token **cur, t_allocs *a);
static t_result parse_pipeline(t_token **cur, t_allocs *a);
static t_result parse_command(t_token **cur, t_allocs *a);
static t_result parse_redirection(t_ast *cmd, t_token **cur, t_allocs *a);

// -- Redirections --------------------------------------------

static t_result parse_redirection(t_ast *cmd, t_token **cur, t_allocs *a) {
  t_token *tok = *cur;
  t_token_type tt = tok->type;
  *cur = advance_token(*cur);
  t_token *file = *cur;
  if (!file || file->type != TOKEN_WORD)
    return create_error(INVALID_REDIRECT);

  // allocate io if needed
  if (!cmd->data.cmd.io) {
    cmd->data.cmd.io = arena_alloc(a->parse_alloc, sizeof(*cmd->data.cmd.io));
    if (!cmd->data.cmd.io)
      return create_error(NO_MEMORY);
    ft_memset(cmd->data.cmd.io, 0, sizeof(*cmd->data.cmd.io));
    cmd->data.cmd.io->in_fd = cmd->data.cmd.io->out_fd = -1;
  }

  if (tt == TOKEN_REDIR_IN) {
    cmd->data.cmd.io->in_file = file->value;
    cmd->data.cmd.io->in_mode = O_RDONLY;
  } else if (tt == TOKEN_HEREDOC) {
    cmd->data.cmd.io->heredoc_delim = file->value;
  } else if (tt == TOKEN_REDIR_OUT) {
    cmd->data.cmd.io->out_file = file->value;
    cmd->data.cmd.io->out_mode = O_WRONLY | O_CREAT | O_TRUNC;
  } else { // TOKEN_APPEND
    cmd->data.cmd.io->out_file = file->value;
    cmd->data.cmd.io->out_mode = O_WRONLY | O_CREAT | O_APPEND;
  }

  *cur = advance_token(file);
  return create_success(NULL);
}

// -- Command & Subshell --------------------------------------

static t_result parse_command(t_token **cur, t_allocs *a) {
  // Subshell?
  if (peek_token_type(*cur) == TOKEN_LPAREN) {
    *cur = advance_token(*cur);
    t_result r = parse_expression(cur, a);
    if (r.is_error) return r;
    if (peek_token_type(*cur) != TOKEN_RPAREN)
      return create_error(INVALID_SYNTAX);
    *cur = advance_token(*cur);
    t_ast *n = ast_new_node(NODE_SUBSHELL, a);
    if (!n) return create_error(NO_MEMORY);
    n->data.sub = r.data.value;
    return create_success(n);
  }

  // Simple command
  t_ast *n = ast_new_node(NODE_CMD, a);
  if (!n) return create_error(NO_MEMORY);
  n->data.cmd.argc = 0;
  size_t cap = 4;
  n->data.cmd.argv = arena_alloc(a->parse_alloc, cap * sizeof(char *));
  if (!n->data.cmd.argv) return create_error(NO_MEMORY);

  // Collect words/subs/wildcards
  while (1) {
    t_token_type tt = peek_token_type(*cur);
    if (tt == TOKEN_WORD || tt == TOKEN_SUBSTITUTION || tt == TOKEN_WILDCARD) {
      if (n->data.cmd.argc + 1 >= cap) {
        cap *= 2;
        char **tmp = arena_alloc(a->parse_alloc, cap * sizeof(char *));
        if (!tmp) return create_error(NO_MEMORY);
        memcpy(tmp, n->data.cmd.argv, n->data.cmd.argc * sizeof(char *));
        n->data.cmd.argv = tmp;
      }
      n->data.cmd.argv[n->data.cmd.argc++] = (*cur)->value;
      *cur = advance_token(*cur);
    }
    else if (tt == TOKEN_REDIR_IN || tt == TOKEN_HEREDOC
          || tt == TOKEN_REDIR_OUT || tt == TOKEN_APPEND) {
      t_result rr = parse_redirection(n, cur, a);
      if (rr.is_error) return rr;
    }
    else break;
  }

  n->data.cmd.argv[n->data.cmd.argc] = NULL;
  return create_success(n);
}

// -- Pipeline -------------------------------------------------

static t_result parse_pipeline(t_token **cur, t_allocs *a) {
  t_result left_r = parse_command(cur, a);
  if (left_r.is_error) return left_r;
  t_ast *left = left_r.data.value;

  while (peek_token_type(*cur) == TOKEN_PIPE) {
    *cur = advance_token(*cur);
    t_result right_r = parse_command(cur, a);
    if (right_r.is_error) return right_r;
    t_ast *pipe_n = ast_new_node(NODE_PIPELINE, a);
    if (!pipe_n) return create_error(NO_MEMORY);
    pipe_n->data.bin.left  = left;
    pipe_n->data.bin.right = right_r.data.value;
    left = pipe_n;
  }

  return create_success(left);
}

// -- Logical AND / OR ----------------------------------------

static t_result parse_logical_and(t_token **cur, t_allocs *a) {
  t_result l = parse_pipeline(cur, a);
  if (l.is_error) return l;
  t_ast *left = l.data.value;

  while (peek_token_type(*cur) == TOKEN_AND) {
    *cur = advance_token(*cur);
    t_result r = parse_pipeline(cur, a);
    if (r.is_error) return r;
    t_ast *n = ast_new_node(NODE_AND, a);
    if (!n) return create_error(NO_MEMORY);
    n->data.bin.left  = left;
    n->data.bin.right = r.data.value;
    left = n;
  }

  return create_success(left);
}

static t_result parse_logical_or(t_token **cur, t_allocs *a) {
  t_result l = parse_logical_and(cur, a);
  if (l.is_error) return l;
  t_ast *left = l.data.value;

  while (peek_token_type(*cur) == TOKEN_OR) {
    *cur = advance_token(*cur);
    t_result r = parse_logical_and(cur, a);
    if (r.is_error) return r;
    t_ast *n = ast_new_node(NODE_OR, a);
    if (!n) return create_error(NO_MEMORY);
    n->data.bin.left  = left;
    n->data.bin.right = r.data.value;
    left = n;
  }

  return create_success(left);
}

// -- Expression & Entry Point --------------------------------

static t_result parse_expression(t_token **cur, t_allocs *a) {
  t_result l = parse_logical_or(cur, a);
  if (l.is_error) return l;
  t_ast *node = l.data.value;

  // background?
  if (peek_token_type(*cur) == TOKEN_BG) {
    *cur = advance_token(*cur);
    t_ast *bg = ast_new_node(NODE_BACKGROUND, a);
    if (!bg) return create_error(NO_MEMORY);
    bg->data.bin.left  = node;
    bg->data.bin.right = NULL;
    node = bg;
  }
  return create_success(node);
}

t_result parse_cmdln(char *cmd, t_mshell *shell, t_allocs *a) {
  t_result lx = lex_cmdln(cmd, a);
  if (lx.is_error) return lx;

  t_token *cur = lx.data.value;
  t_result pr = parse_expression(&cur, a);
  if (pr.is_error) return pr;
  if (peek_token_type(cur) != TOKEN_EOF)
    return create_error(INVALID_SYNTAX);

  shell->ast = pr.data.value;
  return create_success(shell->ast);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sngantch <sngantch@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 18:55:23 by hmensah-          #+#    #+#             */
/*   Updated: 2025/05/25 21:30:50 by sngantch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "../minishell.h"

typedef struct s_exec_ctx
{
	t_ast		*node;
	t_mshell	*shell;
	t_allocs	*allocs;
	t_table		*table;
	int			in_fd;
	int			out_fd;
}	t_exec_ctx;

typedef struct s_pipe_ctx
{
	t_ast	*cmds[30];
	int		pipes[30][2];
	int		count;
	int		index;
	pid_t	pid;
}		t_pipe_ctx;

void		walk_ast(t_ast *ast, t_mshell *sh, t_allocs *allcs, t_table *table);
char		*expand_variable(char *str, t_allocs *allocs, t_table *table);
int			collect_heredoc_input(const char *delim, char *temp_file, t_allocs *allocs, t_table *table);
int			space_or_quote(char c);
int			is_valid_var_char(char c);
void		expand_substitutions(t_ast *ast, t_allocs *allocs, t_table *table);
void		expand_wildcards(t_ast *ast, t_allocs *allocs);
int			handle_pipes(t_ast *ast, t_mshell *sh, t_allocs *allcs, t_table *t);
void		remove_leading_quote(t_ast *ast);
// void		add_full_path(char **argv, char **paths, t_allocs *allocs);
void		close_unused_pipes(t_pipe_ctx *ctx);
void		setup_cmd_redirections(t_in_out *io, int in_fd, int out_fd, t_allocs *allocs, t_table *table);
void		execute_node(t_exec_ctx *ctx);
int			set_in_fds(t_in_out *io, t_allocs *allocs, t_table *table);
int			set_out_fds(t_in_out *io, t_allocs *allocs, t_table *table);
int			create_pipes(t_pipe_ctx *ctx);
int			handle_and_operation(t_ast *ast, t_mshell *shell,
				t_allocs *allocs, t_table *table);
int			handle_or_operation(t_ast *ast, t_mshell *shell,
				t_allocs *allocs, t_table *table);
int			handle_subsh(t_ast *ast, t_mshell *shell, t_allocs *allocs,
				t_table *table);
int			handle_bg(t_ast *ast, t_mshell *shell, t_allocs *allocs,
				t_table *table);
void		set_exit_status(t_mshell *shell, int status);
int			get_exit_status(t_mshell *shell);
int			collect_pipeline_cmds(t_ast *node, t_ast **out_cmds, int max);
void		execute_node(t_exec_ctx *ctx);
int			run_simple_cmd(t_ast *ast, t_mshell *shell, t_allocs *allocs,
				t_table *table);
t_exec_ctx	setup_exec_ctx(t_ast *node, t_mshell *shell, t_allocs *allocs,
				t_table *table);
void	add_full_path(char **argv, t_allocs *allocs, t_table *table);

#endif

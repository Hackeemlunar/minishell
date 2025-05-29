/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 18:55:23 by hmensah-          #+#    #+#             */
/*   Updated: 2025/05/28 20:49:21 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H
# include <dirent.h>
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
int			is_valid_var_char(char c);
void		expand_substitutions(t_ast *ast, t_allocs *allocs, t_table *table);
void		expand_wildcards(t_ast *ast, t_allocs *allocs);
int			handle_pipes(t_ast *ast, t_mshell *sh, t_allocs *allcs, t_table *t);
void		remove_leading_quote(t_ast *ast);
void		close_unused_pipes(t_pipe_ctx *ctx);
void		setup_in_redirections(t_in_out *io, int in_fd, t_allocs *allocs,
				t_table *table);
void		setup_out_redirections(t_in_out *io, int out_fd, t_allocs *allocs,
				t_table *table);
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
void		add_full_path(char **argv, t_allocs *allocs, t_table *table);
char		*process_filename(char *filename, t_allocs *allocs, t_table *table);
int			setup_input_redirection(t_in_out *io, t_allocs *allocs,
				t_table *table);
int			create_tee_process(t_in_out *io, t_allocs *allocs, t_table *table);
int			open_single_output(char *filename, int mode, t_allocs *allocs,
				t_table *table);
void		copy_remaining_part(char *dest, char *start, char *current);
void		build_expanded_string(char *dest, char *str, t_table *table);
void		process_variable_length(char **current, char **start,
				size_t *total_len, t_table *table);
size_t		calculate_total_length(char *current, t_table *table);
char		*handle_quoted_string(char *str, t_allocs *allocs);
void		write_to_multiple_outputs(t_in_out *io, char *data, size_t len);
void		close_multiple_outputs(t_in_out *io);
void		handle_substitution_arg(t_ast *ast, t_allocs *allocs,
				t_table *table, int *i);
int			match_pattern(const char *pattern, const char *str);
int			add_match_to_array(char ***matches, struct dirent *entry,
				t_allocs *allocs, int count);
int			process_directory_entries(DIR *dir, char *pattern,
				char ***matches, t_allocs *allocs);
void		copy_args_before_wildcard(char **new_argv, char **old_argv,
				int idx);
void		copy_matches_to_argv(char **new_argv, char **matches,
				int start_idx, int match_cnt);
void		copy_args_after_wildcard(char **new_argv, char **old_argv,
				int old_idx, int match_cnt);
int			collect_hdoc(const char *delim, char *temp_file, t_allocs *allocs,
				t_table *table);
void		copy_static_part(char **dest, char **start, char *end);
void		process_variable_build(char **current, char **start,
				char **dest, t_table *table);
void		process_escape_sequence(char **current, char **dest, char **start);

#endif

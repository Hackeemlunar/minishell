/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sngantch <sngantch@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 14:25:37 by hmensah-          #+#    #+#             */
/*   Updated: 2025/05/25 21:26:13 by sngantch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static void	exec_cmd_node(t_exec_ctx *ctx)
{
	setup_cmd_redirections(ctx->node->data.cmd_node.io,
		ctx->in_fd, ctx->out_fd, ctx->allocs, ctx->table);
	if (!ctx->node->data.cmd_node.argv)
		return ;
	expand_substitutions(ctx->node, ctx->allocs, ctx->table);
	remove_leading_quote(ctx->node);
	add_full_path(ctx->node->data.cmd_node.argv, ctx->allocs, ctx->table);
	execve(ctx->node->data.cmd_node.argv[0], ctx->node->data.cmd_node.argv,
		ctx->shell->env);
	if (errno == ENOENT)
	{
		ft_putstr_fd(ctx->node->data.cmd_node.argv[0], 2);
		ft_putendl_fd(": command not found", 2);
		exit(127);
	}
	perror("execve");
	exit(126);
}

static void	exec_subshell_node(t_exec_ctx *ctx)
{
	if (ctx->in_fd != STDIN_FILENO)
	{
		if (dup2(ctx->in_fd, STDIN_FILENO) < 0)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		close(ctx->in_fd);
	}
	if (ctx->out_fd != STDOUT_FILENO)
	{
		if (dup2(ctx->out_fd, STDOUT_FILENO) < 0)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		close(ctx->out_fd);
	}
	walk_ast(ctx->node->data.sub,
		ctx->shell, ctx->allocs, ctx->table);
	exit(EXIT_SUCCESS);
}

void	execute_node(t_exec_ctx *ctx)
{
	if (!ctx->node)
		return ;
	if (ctx->node->type == NODE_CMD)
		exec_cmd_node(ctx);
	else if (ctx->node->type == NODE_SUBSHELL)
		exec_subshell_node(ctx);
	else
	{
		ft_putstr_fd("Unsupported node type in pipeline: ", 2);
		ft_putnbr_fd(ctx->node->type, 2);
		ft_putchar_fd('\n', 2);
		exit(EXIT_FAILURE);
	}
}

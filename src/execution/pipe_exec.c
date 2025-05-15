/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 14:10:10 by hmensah-          #+#    #+#             */
/*   Updated: 2025/05/15 14:19:15 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static void	setup_child_pipes(t_pipe_ctx *ctx, t_exec_ctx *exec_ctx)
{
	if (ctx->index == 0)
		exec_ctx->in_fd = STDIN_FILENO;
	else
		exec_ctx->in_fd = ctx->pipes[ctx->index - 1][0];
	if (ctx->index == ctx->count - 1)
		exec_ctx->out_fd = STDOUT_FILENO;
	else
		exec_ctx->out_fd = ctx->pipes[ctx->index][1];
	close_unused_pipes(ctx);
}

static int	wait_for_children(t_pipe_ctx *ctx, t_mshell *shell)
{
	int	i;
	int	status;
	int	last_status;

	i = 0;
	last_status = 0;
	while (i < ctx->count)
	{
		if (wait(&status) < 0)
			return (perror("wait"), 1);
		if (i == ctx->count - 1)
		{
			if (WIFEXITED(status))
				last_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				last_status = 128 + WTERMSIG(status);
			else
				last_status = 1;
			set_exit_status(shell, last_status);
		}
		i++;
	}
	return (last_status);
}

static int	execute_pipeline_cmd(t_pipe_ctx *ctx, t_mshell *shell,
								t_allocs *allocs, t_table *table)
{
	t_exec_ctx	exec_ctx;

	ctx->pid = fork();
	if (ctx->pid < 0)
	{
		perror("fork");
		return (1);
	}
	if (ctx->pid == 0)
	{
		exec_ctx = setup_exec_ctx(ctx->cmds[ctx->index], shell, allocs, table);
		setup_child_pipes(ctx, &exec_ctx);
		execute_node(&exec_ctx);
		exit(EXIT_FAILURE);
	}
	if (ctx->index > 0)
		close(ctx->pipes[ctx->index - 1][0]);
	if (ctx->index < ctx->count - 1)
		close(ctx->pipes[ctx->index][1]);
	return (0);
}

static int	execute_pipeline_cmds(t_pipe_ctx *ctx, t_mshell *shell,
								t_allocs *allocs, t_table *table)
{
	while (ctx->index < ctx->count)
	{
		if (execute_pipeline_cmd(ctx, shell, allocs, table))
			return (1);
		ctx->index++;
	}
	return (0);
}

int	handle_pipes(t_ast *ast, t_mshell *shell, t_allocs *allocs, t_table *table)
{
	t_pipe_ctx	ctx;

	ctx.count = 0;
	ctx.index = 0;
	ctx.count = collect_pipeline_cmds(ast, ctx.cmds, 30);
	if (ctx.count < 0)
		return (1);
	if (create_pipes(&ctx))
		return (1);
	if (execute_pipeline_cmds(&ctx, shell, allocs, table))
		return (1);
	return (wait_for_children(&ctx, shell));
}

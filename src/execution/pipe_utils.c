#include "executor.h"

static int collect_pipeline_left(t_ast *node, t_ast **out_cmds, int max)
{
	int left_count;
	
	left_count = collect_pipeline_cmds(node->data.bin_op_node.left, out_cmds, max);
	if (left_count < 0 || left_count >= max)
		return (-1);
	return (left_count);
}

static int collect_pipeline_right(t_ast *node, t_ast **out_cmds, int left_count, int max)
{
	int right_count;
	
	right_count = collect_pipeline_cmds(node->data.bin_op_node.right, 
									  out_cmds + left_count, max);
	if (right_count < 0)
		return (-1);
	return (left_count + right_count);
}

int collect_pipeline_cmds(t_ast *node, t_ast **out_cmds, int max)
{
	int left_count;

	if (node->type == NODE_PIPELINE)
	{
		left_count = collect_pipeline_left(node, out_cmds, max);
		if (left_count < 0)
			return (-1);
		return collect_pipeline_right(node, out_cmds, left_count, max);
	}
	out_cmds[0] = node;
	return (1);
}

t_exec_ctx setup_exec_ctx(t_ast *node, t_mshell *shell, 
						t_allocs *allocs, t_table *table)
{
	t_exec_ctx ctx;

	ctx.node = node;
	ctx.shell = shell;
	ctx.allocs = allocs;
	ctx.table = table;
	ctx.in_fd = STDIN_FILENO;
	ctx.out_fd = STDOUT_FILENO;
	return (ctx);
}

void	close_unused_pipes(t_pipe_ctx *ctx)
{
	int j;

	j = 0;
	while (j < ctx->count - 1)
	{
		if (j != ctx->index - 1)
			close(ctx->pipes[j][0]);
		if (j != ctx->index)
			close(ctx->pipes[j][1]);
		j++;
	}
}

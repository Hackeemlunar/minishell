#include "executor.h"

static int collect_pipeline_cmds(t_ast *node, t_ast **out_cmds, int max);

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

static int collect_pipeline_cmds(t_ast *node, t_ast **out_cmds, int max)
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


static void setup_cmd_redirections(t_in_out *io, int in_fd, int out_fd)
{
	int has_in = io && ((io->in_mode == 0 && io->in_file) || (io->in_mode == 1 && io->heredoc_delim));
	int has_out = io && ((io->out_mode == 0 || io->out_mode == 1) && io->out_file);
	if (has_in) {
		if (in_fd != STDIN_FILENO)
			close(in_fd);
		if (set_in_fds(io))
			exit(EXIT_FAILURE);
	} else if (in_fd != STDIN_FILENO) {
		if (dup2(in_fd, STDIN_FILENO) < 0) {
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		close(in_fd);
	}
	if (has_out) {
		if (out_fd != STDOUT_FILENO)
			close(out_fd);
		if (set_out_fds(io))
			exit(EXIT_FAILURE);
	} else if (out_fd != STDOUT_FILENO) {
		if (dup2(out_fd, STDOUT_FILENO) < 0) {
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		close(out_fd);
	}
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

static void execute_node(t_exec_ctx *ctx)
{
	if (!ctx->node)
		return;
	if (ctx->node->type == NODE_CMD) {
		setup_cmd_redirections(ctx->node->data.cmd_node.io, ctx->in_fd, ctx->out_fd);
		expand_substitutions(ctx->node, ctx->allocs, ctx->table);
		remove_leading_quote(ctx->node);
		add_full_path(ctx->node->data.cmd_node.argv, ctx->shell->paths, ctx->allocs);
		execve(ctx->node->data.cmd_node.argv[0], ctx->node->data.cmd_node.argv, ctx->shell->env);
		if (errno == ENOENT) {
			ft_putstr_fd(ctx->node->data.cmd_node.argv[0], 2);
			ft_putendl_fd(": command not found", 2);
			exit(127);
		}
		perror("execve");
		exit(126);
	} else if (ctx->node->type == NODE_SUBSHELL) {
		if (ctx->in_fd != STDIN_FILENO) {
			if (dup2(ctx->in_fd, STDIN_FILENO) < 0) {
				perror("dup2");
				exit(EXIT_FAILURE);
			}
			close(ctx->in_fd);
		}
		if (ctx->out_fd != STDOUT_FILENO) {
			if (dup2(ctx->out_fd, STDOUT_FILENO) < 0) {
				perror("dup2");
				exit(EXIT_FAILURE);
			}
			close(ctx->out_fd);
		}
		walk_ast(ctx->node->data.sub, ctx->shell, ctx->allocs, ctx->table);
		exit(EXIT_SUCCESS);
	} else {
		ft_putstr_fd("Unsupported node type in pipeline: ", 2);
		ft_putnbr_fd(ctx->node->type, 2);
		ft_putchar_fd('\n', 2);
		exit(EXIT_FAILURE);
	}
}

static int create_pipes(t_pipe_ctx *ctx)
{
	int i;

	i = 0;
	while (i < ctx->count - 1)
	{
		if (pipe(ctx->pipes[i]) < 0)
		{
			perror("pipe");
			while (--i >= 0)
			{
				close(ctx->pipes[i][0]);
				close(ctx->pipes[i][1]);
			}
			return (1);
		}
		i++;
	}
	return (0);
}

static void close_unused_pipes(t_pipe_ctx *ctx)
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

static void setup_child_pipes(t_pipe_ctx *ctx, t_exec_ctx *exec_ctx)
{
	exec_ctx->in_fd = (ctx->index == 0) ? STDIN_FILENO : ctx->pipes[ctx->index - 1][0];
	exec_ctx->out_fd = (ctx->index == ctx->count - 1) ? STDOUT_FILENO : ctx->pipes[ctx->index][1];
	close_unused_pipes(ctx);
}

static int wait_for_children(t_pipe_ctx *ctx, t_mshell *shell)
{
	int i;
	int status;
	int last_status;

	i = 0;
	last_status = 0;
	while (i < ctx->count)
	{
		if (wait(&status) < 0)
		{
			ft_putendl_fd("wait: Resource temporarily unavailable", 2);
			return (1);
		}
		
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

static int execute_pipeline_cmd(t_pipe_ctx *ctx, t_mshell *shell,
							  t_allocs *allocs, t_table *table)
{
	t_exec_ctx exec_ctx;

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

static int execute_pipeline_cmds(t_pipe_ctx *ctx, t_mshell *shell,
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

int handle_pipes(t_ast *ast, t_mshell *shell, t_allocs *allocs, t_table *table)
{
	t_pipe_ctx ctx;

	ctx.count = 0;
	ctx.index = 0;
	ctx.count = collect_pipeline_cmds(ast, ctx.cmds, 30);
	if (ctx.count < 0)
		return (1);
	if (create_pipes(&ctx))
		return (1);
	if (execute_pipeline_cmds(&ctx, shell, allocs, table))
		return (1);
	return wait_for_children(&ctx, shell);
}


#include "executor.h"

void    execute_node(t_exec_ctx *ctx)
{
	if (!ctx->node)
		return;
	if (ctx->node->type == NODE_CMD)
    {
		setup_cmd_redirections(ctx->node->data.cmd_node.io, ctx->in_fd, ctx->out_fd);
		expand_substitutions(ctx->node, ctx->allocs, ctx->table);
		remove_leading_quote(ctx->node);
		add_full_path(ctx->node->data.cmd_node.argv, ctx->shell->paths, ctx->allocs);
		execve(ctx->node->data.cmd_node.argv[0], ctx->node->data.cmd_node.argv, ctx->shell->env);
		if (errno == ENOENT)
        {
			ft_putstr_fd(ctx->node->data.cmd_node.argv[0], 2);
			ft_putendl_fd(": command not found", 2);
			exit(127);
		}
		perror("execve");
		exit(126);
	}
    else if (ctx->node->type == NODE_SUBSHELL)
    {
		if (ctx->in_fd != STDIN_FILENO) {
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
		walk_ast(ctx->node->data.sub, ctx->shell, ctx->allocs, ctx->table);
		exit(EXIT_SUCCESS);
	}
    else
    {
		ft_putstr_fd("Unsupported node type in pipeline: ", 2);
		ft_putnbr_fd(ctx->node->type, 2);
		ft_putchar_fd('\n', 2);
		exit(EXIT_FAILURE);
	}
}

void setup_cmd_redirections(t_in_out *io, int in_fd, int out_fd)
{
	int	has_in;
	int	has_out;

	has_in = io && ((io->in_mode == 0 && io->in_file)
		|| (io->in_mode == 1 && io->heredoc_delim));
	has_out = io && ((io->out_mode == 0 || io->out_mode == 1) && io->out_file);
	if (has_in)
	{
		if (in_fd != STDIN_FILENO)
			close(in_fd);
		if (set_in_fds(io))
			exit(EXIT_FAILURE);
	}
	else if (in_fd != STDIN_FILENO)
	{
		if (dup2(in_fd, STDIN_FILENO) < 0)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		close(in_fd);
	}
	if (has_out)
	{
		if (out_fd != STDOUT_FILENO)
			close(out_fd);
		if (set_out_fds(io))
			exit(EXIT_FAILURE);
	} else if (out_fd != STDOUT_FILENO)
	{
		if (dup2(out_fd, STDOUT_FILENO) < 0)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		close(out_fd);
	}
}

int create_pipes(t_pipe_ctx *ctx)
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 14:27:58 by hmensah-          #+#    #+#             */
/*   Updated: 2025/05/15 14:28:44 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static void	setup_in_redirections(t_in_out *io, int in_fd)
{
	int	has_in;

	has_in = io && ((io->in_mode == 0 && io->in_file)
			|| (io->in_mode == 1 && io->heredoc_delim));
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
}

static void	setup_out_redirections(t_in_out *io, int out_fd)
{
	int	has_out;

	has_out = io && ((io->out_mode == 0 || io->out_mode == 1)
			&& io->out_file);
	if (has_out)
	{
		if (out_fd != STDOUT_FILENO)
			close(out_fd);
		if (set_out_fds(io))
			exit(EXIT_FAILURE);
	}
	else if (out_fd != STDOUT_FILENO)
	{
		if (dup2(out_fd, STDOUT_FILENO) < 0)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		close(out_fd);
	}
}

void	setup_cmd_redirections(t_in_out *io, int in_fd, int out_fd)
{
	setup_in_redirections(io, in_fd);
	setup_out_redirections(io, out_fd);
}

int	create_pipes(t_pipe_ctx *ctx)
{
	int	i;

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

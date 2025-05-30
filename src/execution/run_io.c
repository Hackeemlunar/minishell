/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_io.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 20:34:13 by hmensah-          #+#    #+#             */
/*   Updated: 2025/05/28 20:40:28 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int	set_in_fds(t_in_out *io, t_allocs *allocs, t_table *table)
{
	return (setup_input_redirection(io, allocs, table));
}

static int	setup_multiple_outputs(t_in_out *io, t_allocs *allocs,
	t_table *table)
{
	if (!io->out_redirs->next)
	{
		io->out_fd = open_single_output(io->out_redirs->filename,
				io->out_redirs->mode, allocs, table);
		if (io->out_fd < 0)
			return (perror("open"), 1);
		return (0);
	}
	return (create_tee_process(io, allocs, table));
}

static int	setup_single_output(t_in_out *io, t_allocs *allocs, t_table *table)
{
	char	*original_filename;

	if (io->out_mode == 0 && io->out_file)
	{
		original_filename = io->out_file;
		io->out_fd = open_single_output(original_filename, 0, allocs, table);
		if (io->out_fd < 0)
			return (perror("open"), 1);
		dup2(io->out_fd, STDOUT_FILENO);
	}
	else if (io->out_mode == 1 && io->out_file)
	{
		original_filename = io->out_file;
		io->out_fd = open_single_output(original_filename, 1, allocs, table);
		if (io->out_fd < 0)
			return (perror("open"), 1);
		dup2(io->out_fd, STDOUT_FILENO);
	}
	else
		io->out_fd = STDOUT_FILENO;
	return (0);
}

int	set_out_fds(t_in_out *io, t_allocs *allocs, t_table *table)
{
	if (io->out_redirs)
	{
		if (setup_multiple_outputs(io, allocs, table))
			return (1);
		dup2(io->out_fd, STDOUT_FILENO);
		return (0);
	}
	return (setup_single_output(io, allocs, table));
}

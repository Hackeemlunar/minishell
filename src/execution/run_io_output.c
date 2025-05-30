/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_io_output.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 00:00:00 by hmensah-          #+#    #+#             */
/*   Updated: 2025/01/27 00:00:00 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	write_to_multiple_outputs(t_in_out *io, char *data, size_t len)
{
	t_redir	*redir;

	if (!io || !io->out_redirs || !io->out_redirs->next)
		return ;
	redir = io->out_redirs->next;
	while (redir)
	{
		if (redir->fd > 0)
			write(redir->fd, data, len);
		redir = redir->next;
	}
}

void	close_multiple_outputs(t_in_out *io)
{
	t_redir	*redir;

	if (!io || !io->out_redirs)
		return ;
	redir = io->out_redirs;
	while (redir)
	{
		if (redir->fd > 0)
			close(redir->fd);
		redir = redir->next;
	}
}

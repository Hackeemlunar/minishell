/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_io_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 14:09:59 by hmensah-          #+#    #+#             */
/*   Updated: 2025/05/29 14:09:59 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static void	open_redir_files(t_redir *redir, t_allocs *allocs, t_table *table)
{
	while (redir)
	{
		redir->fd = open_single_output(redir->filename, redir->mode,
				allocs, table);
		if (redir->fd < 0)
			exit(1);
		redir = redir->next;
	}
}

static void	write_to_all_redirs(t_redir *redir, char *buffer, ssize_t bytes)
{
	while (redir)
	{
		write(redir->fd, buffer, bytes);
		redir = redir->next;
	}
}

static void	handle_tee_child(int pipe_fd[2], t_in_out *io,
							t_allocs *allocs, t_table *table)
{
	t_redir	*redir;
	char	buffer[4096];
	ssize_t	bytes;

	close(pipe_fd[1]);
	redir = io->out_redirs;
	open_redir_files(redir, allocs, table);
	bytes = read(pipe_fd[0], buffer, sizeof(buffer));
	while (bytes > 0)
	{
		redir = io->out_redirs;
		write_to_all_redirs(redir, buffer, bytes);
		bytes = read(pipe_fd[0], buffer, sizeof(buffer));
	}
	close(pipe_fd[0]);
	exit(0);
}

int	create_tee_process(t_in_out *io, t_allocs *allocs, t_table *table)
{
	int		pipe_fd[2];
	pid_t	pid;

	if (pipe(pipe_fd) < 0)
		return (perror("pipe"), 1);
	pid = fork();
	if (pid < 0)
		return (perror("fork"), 1);
	if (pid == 0)
		handle_tee_child(pipe_fd, io, allocs, table);
	close(pipe_fd[0]);
	io->out_fd = pipe_fd[1];
	io->tee_pid = pid;
	return (0);
}

int	open_single_output(char *filename, int mode, t_allocs *allocs,
	t_table *table)
{
	char	*processed_filename;
	int		fd;
	int		flags;
	int		has_dollar;

	has_dollar = (strchr(filename, '$') != NULL);
	processed_filename = process_filename(filename, allocs, table);
	if (mode == 0)
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	else
		flags = O_WRONLY | O_CREAT | O_APPEND;
	fd = open(processed_filename, flags, 0644);
	if (processed_filename != filename && !has_dollar)
		free(processed_filename);
	return (fd);
}

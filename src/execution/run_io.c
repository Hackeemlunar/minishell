/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_io.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 14:37:21 by hmensah-          #+#    #+#             */
/*   Updated: 2025/05/15 14:38:15 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int	collect_heredoc_input(const char *delim, char *temp_file)
{
	int		fd;
	char	*line;

	fd = open(temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (perror("error in heredoc"), -1);
	while (1)
	{
		line = readline("heredoc>");
		if (!line)
		{
			write(STDERR_FILENO, "\n", 1);
			break ;
		}
		if (ft_strcmp(line, delim) == 0)
		{
			free(line);
			break ;
		}
		ft_putendl_fd(line, fd);
		free(line);
	}
	return (close(fd), 0);
}

int	set_in_fds(t_in_out *io)
{
	if (io->in_mode == 1 && io->heredoc_delim)
	{
		if (collect_heredoc_input(io->heredoc_delim, "/tmp/heredoc") < 0)
			return (1);
		io->in_fd = open("/tmp/heredoc", O_RDONLY);
		if (io->in_fd < 0)
			return (perror("open"), 1);
		dup2(io->in_fd, STDIN_FILENO);
	}
	else if (io->in_mode == 0 && io->in_file)
	{
		io->in_fd = open(io->in_file, O_RDONLY);
		if (io->in_fd < 0)
			return (perror("open"), 1);
		dup2(io->in_fd, STDIN_FILENO);
	}
	else
		io->in_fd = STDIN_FILENO;
	return (0);
}

int	set_out_fds(t_in_out *io)
{
	if (io->out_mode == 0 && io->out_file)
	{
		io->out_fd = open(io->out_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (io->out_fd < 0)
			return (perror("open"), 1);
		dup2(io->out_fd, STDOUT_FILENO);
	}
	else if (io->out_mode == 1 && io->out_file)
	{
		io->out_fd = open(io->out_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (io->out_fd < 0)
			return (perror("open"), 1);
		dup2(io->out_fd, STDOUT_FILENO);
	}
	else
		io->out_fd = STDOUT_FILENO;
	return (0);
}

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

static int	collect_hdoc(const char *delim, char *temp_file, t_allocs *allocs,
	t_table *table)
{
	int		fd;
	char	*line;
	char	*expanded_line;

	fd = open(temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (perror("error in heredoc"), -1);
	while (1)
	{
		line = readline("> ");
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
		if (strchr(line, '$') != NULL)
		{
			expanded_line = expand_variable(line, allocs, table);
			if (expanded_line)
			{
				ft_putendl_fd(expanded_line, fd);
				free(expanded_line);
			}
			else
				ft_putendl_fd(line, fd);
		}
		else
			ft_putendl_fd(line, fd);
		free(line);
	}
	return (close(fd), 0);
}

static char	*process_filename(char *filename, t_allocs *allocs, t_table *table)
{
	char	*processed;
	char	*expanded;
	int		len;

	if (!filename || !*filename)
		return (filename);
	if (strchr(filename, '$') != NULL)
	{
		expanded = expand_variable(filename, allocs, table);
		if (expanded)
			return (expanded);
	}
	if (filename[0] != '"' && filename[0] != '\'')
		return (filename);
	processed = ft_strdup(filename + 1);
	if (!processed)
		return (filename);
	len = ft_strlen(processed);
	if (len > 0 && (processed[len - 1] == '"' || processed[len - 1] == '\''))
		processed[len - 1] = '\0';
	return (processed);
}

int	set_in_fds(t_in_out *io, t_allocs *allocs, t_table *table)
{
	char	*filename;
	char	*original_filename;
	int		has_dollar;

	if (io->in_mode == 1 && io->heredoc_delim)
	{
		if (collect_hdoc(io->heredoc_delim, "/tmp/heredoc", allocs, table) < 0)
			return (1);
		io->in_fd = open("/tmp/heredoc", O_RDONLY);
		if (io->in_fd < 0)
			return (perror("open"), 1);
		dup2(io->in_fd, STDIN_FILENO);
	}
	else if (io->in_mode == 0 && io->in_file)
	{
		original_filename = io->in_file;
		has_dollar = (strchr(original_filename, '$') != NULL);
		filename = process_filename(original_filename, allocs, table);
		io->in_fd = open(filename, O_RDONLY);
		if (filename != original_filename && !has_dollar)
			free(filename);
		if (io->in_fd < 0)
			return (perror("open"), 1);
		dup2(io->in_fd, STDIN_FILENO);
	}
	else
		io->in_fd = STDIN_FILENO;
	return (0);
}

static int	open_single_output(char *filename, int mode, t_allocs *allocs,
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

static int	create_tee_process(t_in_out *io, t_allocs *allocs, t_table *table)
{
	int		pipe_fd[2];
	pid_t	pid;
	t_redir	*redir;
	char	buffer[4096];
	ssize_t	bytes;

	if (pipe(pipe_fd) < 0)
		return (perror("pipe"), 1);
	pid = fork();
	if (pid < 0)
		return (perror("fork"), 1);
	if (pid == 0)
	{
		close(pipe_fd[1]);
		redir = io->out_redirs;
		while (redir)
		{
			redir->fd = open_single_output(redir->filename, redir->mode,
					allocs, table);
			if (redir->fd < 0)
				exit(1);
			redir = redir->next;
		}
		while ((bytes = read(pipe_fd[0], buffer, sizeof(buffer))) > 0)
		{
			redir = io->out_redirs;
			while (redir)
			{
				write(redir->fd, buffer, bytes);
				redir = redir->next;
			}
		}
		close(pipe_fd[0]);
		exit(0);
	}
	close(pipe_fd[0]);
	io->out_fd = pipe_fd[1];
	io->tee_pid = pid;
	return (0);
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

int	set_out_fds(t_in_out *io, t_allocs *allocs, t_table *table)
{
	char	*original_filename;

	if (io->out_redirs)
	{
		if (setup_multiple_outputs(io, allocs, table))
			return (1);
		dup2(io->out_fd, STDOUT_FILENO);
		return (0);
	}
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


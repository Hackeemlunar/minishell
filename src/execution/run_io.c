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

int	set_out_fds(t_in_out *io, t_allocs *allocs, t_table *table)
{
	char	*filename;
	char	*original_filename;
	int		has_dollar;

	if (io->out_mode == 0 && io->out_file)
	{
		original_filename = io->out_file;
		has_dollar = (strchr(original_filename, '$') != NULL);
		filename = process_filename(original_filename, allocs, table);
		io->out_fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (filename != original_filename && !has_dollar)
			free(filename);
		if (io->out_fd < 0)
			return (perror("open"), 1);
		dup2(io->out_fd, STDOUT_FILENO);
	}
	else if (io->out_mode == 1 && io->out_file)
	{
		original_filename = io->out_file;
		has_dollar = (strchr(original_filename, '$') != NULL);
		filename = process_filename(original_filename, allocs, table);
		io->out_fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (filename != original_filename && !has_dollar)
			free(filename);
		if (io->out_fd < 0)
			return (perror("open"), 1);
		dup2(io->out_fd, STDOUT_FILENO);
	}
	else
		io->out_fd = STDOUT_FILENO;
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_io_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 14:09:59 by hmensah-          #+#    #+#             */
/*   Updated: 2025/05/29 14:09:59 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static char	*remove_quotes(char *filename)
{
	char	*processed;
	int		len;

	processed = ft_strdup(filename + 1);
	if (!processed)
		return (filename);
	len = ft_strlen(processed);
	if (len > 0 && (processed[len - 1] == '"' || processed[len - 1] == '\''))
		processed[len - 1] = '\0';
	return (processed);
}

char	*process_filename(char *filename, t_allocs *allocs, t_table *table)
{
	char	*expanded;

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
	return (remove_quotes(filename));
}

static int	handle_heredoc(t_in_out *io, t_allocs *allocs, t_table *table)
{
	if (collect_hdoc(io->heredoc_delim, "/tmp/heredoc", allocs, table) < 0)
		return (1);
	io->in_fd = open("/tmp/heredoc", O_RDONLY);
	if (io->in_fd < 0)
		return (perror("open"), 1);
	dup2(io->in_fd, STDIN_FILENO);
	return (0);
}

static int	handle_input_file(t_in_out *io, t_allocs *allocs, t_table *table)
{
	char	*original_filename;
	char	*filename;
	int		has_dollar;

	original_filename = io->in_file;
	has_dollar = (strchr(original_filename, '$') != NULL);
	filename = process_filename(original_filename, allocs, table);
	io->in_fd = open(filename, O_RDONLY);
	if (filename != original_filename && !has_dollar)
		free(filename);
	if (io->in_fd < 0)
		return (perror("open"), 1);
	dup2(io->in_fd, STDIN_FILENO);
	return (0);
}

int	setup_input_redirection(t_in_out *io, t_allocs *allocs, t_table *table)
{
	if (io->in_mode == 1 && io->heredoc_delim)
		return (handle_heredoc(io, allocs, table));
	else if (io->in_mode == 0 && io->in_file)
		return (handle_input_file(io, allocs, table));
	else
		io->in_fd = STDIN_FILENO;
	return (0);
}

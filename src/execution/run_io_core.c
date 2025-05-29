/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_io_core.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 20:34:13 by hmensah-          #+#    #+#             */
/*   Updated: 2025/05/29 20:40:28 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static void	process_heredoc_line(char *line, int fd, t_allocs *allocs,
								t_table *table)
{
	char	*expanded_line;

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
}

int	collect_hdoc(const char *delim, char *temp_file, t_allocs *allocs,
	t_table *table)
{
	int		fd;
	char	*line;

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
		process_heredoc_line(line, fd, allocs, table);
		free(line);
	}
	return (close(fd), 0);
}

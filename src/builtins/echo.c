/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 20:57:50 by sngantch          #+#    #+#             */
/*   Updated: 2025/05/24 15:22:25 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

static inline int open_fd(t_ast *node)
{
	t_in_out	*io;
	int			fd;

	io = node->data.cmd_node.io;
	if ((io->out_mode == 0) && io->out_file)
		fd = open(io->out_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if ((io->out_mode == 1) && io->out_file)
		fd = open(io->out_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = STDOUT_FILENO;
	return (fd);
}

int	echo(t_ast *node)
{
	int		i;
	int		new_line;
	int		fd;
	char	**argv;

	i = 1;
	new_line = 1;
	argv = node->data.cmd_node.argv;
	fd = open_fd(node);
	if (fd < 0)
		return (perror(node->data.cmd_node.io->out_file), 1);
	if (argv[i] && (ft_strncmp(argv[i], "-n", 2) == 0))
	{
		new_line = 0;
		i++;
	}
	while (i < node->data.cmd_node.argc)
	{
		ft_putstr_fd(argv[i], fd);
		if (argv[(i++) + 1])
			ft_putstr_fd(" ", fd);
	}
	if (new_line)
		printf("\n");
	return (0);
}

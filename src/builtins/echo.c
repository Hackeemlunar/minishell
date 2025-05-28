/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sngantch <sngantch@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 20:57:50 by sngantch          #+#    #+#             */
/*   Updated: 2025/05/27 18:28:04 by sngantch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int	echo(t_ast *node)
{
	int		i;
	int		new_line;
	char	**argv;

	i = 1;
	new_line = 1;
	argv = node->data.cmd_node.argv;
	if (argv[i] && (ft_strncmp(argv[i], "-n", 2) == 0))
	{
		new_line = 0;
		i++;
	}
	while (i < node->data.cmd_node.argc)
	{
		ft_putstr_fd(argv[i], STDOUT_FILENO);
		if (argv[(i++) + 1])
			ft_putstr_fd(" ", STDOUT_FILENO);
	}
	if (new_line)
		ft_putstr_fd("\n", STDOUT_FILENO);
	return (0);
}

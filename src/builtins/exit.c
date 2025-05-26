/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sngantch <sngantch@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 20:57:58 by sngantch          #+#    #+#             */
/*   Updated: 2025/05/26 17:40:20 by sngantch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

void	ft_exit(t_allocs *allocs, t_table *table, t_ast *node, t_mshell *shell)
{ 
	int	exit_status;

	if (node && node->data.cmd_node.argv && node->data.cmd_node.argv[1])
	{
		if (node->data.cmd_node.argv[2])
		{
			ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
			set_exit_status(shell, 1);
			return ;
		}
		exit_status = ft_atoi(node->data.cmd_node.argv[1]);
		if (exit_status < 0 || exit_status > 255)
		{
			ft_putstr_fd("minishell: exit: numeric argument required\n", STDERR_FILENO);
			set_exit_status(shell, 2);
			return ;
		}
	}
	else
		exit_status = get_exit_status(shell);
	
	clean_mshell(allocs, table);
	printf("exit\n");
	exit(exit_status);
}

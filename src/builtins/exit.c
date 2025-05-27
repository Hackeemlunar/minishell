/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sngantch <sngantch@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 20:57:58 by sngantch          #+#    #+#             */
/*   Updated: 2025/05/27 18:49:29 by sngantch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int	is_valid_digit(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

void	set_digit_exit_status(t_ast *node, int *exit_status, t_mshell *shell)
{
	*exit_status = ft_atoi(node->data.cmd_node.argv[1]);
	if (*exit_status < 0 || *exit_status > 255)
		set_exit_status(shell, *exit_status % 256);
	else
		set_exit_status(shell, *exit_status);
}

void	ft_exit(t_allocs *allocs, t_table *table, t_ast *node, t_mshell *shell)
{
	int	exit_status;

	if (node && node->data.cmd_node.argv && node->data.cmd_node.argv[1])
	{
		if (node->data.cmd_node.argv[2])
		{
			ft_putstr_fd("minishell: exit: too many arguments\n",
				STDERR_FILENO);
			set_exit_status(shell, 1);
			return ;
		}
		if (!is_valid_digit(node->data.cmd_node.argv[1]))
		{
			ft_putstr_fd("minishell: exit: numeric argument required\n",
				STDERR_FILENO);
			set_exit_status(shell, 255);
		}
		else
			set_digit_exit_status(node, &exit_status, shell);
	}
	else
		exit_status = get_exit_status(shell);
	clean_mshell(allocs, table);
	printf("exit\n");
	exit(shell->exit_status);
}

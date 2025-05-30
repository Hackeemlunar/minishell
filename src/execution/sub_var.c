/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sub_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 14:39:05 by hmensah-          #+#    #+#             */
/*   Updated: 2025/05/24 18:09:24 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	expand_substitutions(t_ast *ast, t_allocs *allocs, t_table *table)
{
	int	i;

	if (ast->type != NODE_CMD)
		return ;
	i = 0;
	while (i < ast->data.cmd_node.argc)
		handle_substitution_arg(ast, allocs, table, &i);
}

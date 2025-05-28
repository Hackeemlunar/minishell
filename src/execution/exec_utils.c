/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 14:06:52 by hmensah-          #+#    #+#             */
/*   Updated: 2025/05/28 19:59:09 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int	is_valid_var_char(char c)
{
	return (ft_isalnum(c) || c == '_' || c == '?');
}

void	remove_leading_quote(t_ast *ast)
{
	int		idx;
	int		len;
	char	quote_type;

	idx = 0;
	while (idx < ast->data.cmd_node.argc)
	{
		if (ast->data.cmd_node.argv[idx][0] == '"'
			|| ast->data.cmd_node.argv[idx][0] == '\'')
		{
			quote_type = ast->data.cmd_node.argv[idx][0];
			ast->data.cmd_node.argv[idx] = ast->data.cmd_node.argv[idx] + 1;
			len = ft_strlen(ast->data.cmd_node.argv[idx]);
			if (len > 0 && ast->data.cmd_node.argv[idx][len - 1] == quote_type)
			{
				ast->data.cmd_node.argv[idx][len - 1] = '\0';
			}
		}
		idx++;
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sub_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 14:39:05 by hmensah-          #+#    #+#             */
/*   Updated: 2025/05/15 14:40:56 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static char	*expand_var(t_allocs *allocs, t_result value)
{
	char	*expanded;

	expanded = arena_alloc(allocs->exec_alloc, ft_strlen(value.data.value) + 1);
	ft_strlcpy(expanded, value.data.value, ft_strlen(value.data.value) + 1);
	return (expanded);
}

static void	shift_arguments(t_ast *ast, int start)
{
	int	j;

	j = start;
	while (j < ast->data.cmd_node.argc - 1)
	{
		ast->data.cmd_node.argv[j] = ast->data.cmd_node.argv[j + 1];
		j++;
	}
	ast->data.cmd_node.argc--;
	ast->data.cmd_node.argv[ast->data.cmd_node.argc] = NULL;
}

static char	*get_variable_name(char **argv, int i)
{
	char	*var_arg;
	char	*var_name;
	size_t	len;

	var_arg = argv[i];
	len = ft_strlen(var_arg);
	if (len >= 2 && ((var_arg[0] == '"' && var_arg[len - 1] == '"')
			|| (var_arg[0] == '\'' && var_arg[len - 1] == '\'')))
		var_name = ft_substr(var_arg, 1, len - 2);
	else
		var_name = ft_strdup(var_arg);
	return (var_name);
}

static void	replace_vari(t_ast *ast, t_allocs *allocs, t_table *table, int *i)
{
	char		*var_name;
	char		*expanded;
	t_result	value;

	if (*i + 1 >= ast->data.cmd_node.argc)
	{
		printf("Error: no variable after $\n");
		(*i)++;
		return ;
	}
	var_name = get_variable_name(ast->data.cmd_node.argv, *i + 1);
	value = get_env(table, var_name);
	free(var_name);
	if (value.is_error)
	{
		printf("Unknown variable: %s\n", ast->data.cmd_node.argv[*i + 1]);
		*i += 2;
		return ;
	}
	expanded = expand_var(allocs, value);
	ast->data.cmd_node.argv[*i] = expanded;
	shift_arguments(ast, *i + 1);
}

void	expand_substitutions(t_ast *ast, t_allocs *allocs, t_table *table)
{
	int		i;
	char	*arg;
	char	*expanded;

	if (ast->type != NODE_CMD)
		return ;
	i = 0;
	while (i < ast->data.cmd_node.argc)
	{
		arg = ast->data.cmd_node.argv[i];
		if (arg[0] == '$' && arg[1] == '\0')
		{
			replace_vari(ast, allocs, table, &i);
			continue ;
		}
		else
		{
			expanded = expand_variable(arg, allocs, table);
			if (expanded)
				ast->data.cmd_node.argv[i] = expanded;
			i++;
		}
	}
}

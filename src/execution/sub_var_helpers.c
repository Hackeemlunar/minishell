/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sub_var_helpers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 00:00:00 by hmensah-          #+#    #+#             */
/*   Updated: 2025/01/27 00:00:00 by hmensah-         ###   ########.fr       */
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
	char		*var_key;
	char		*expanded;
	t_result	value;

	if (*i + 1 >= ast->data.cmd_node.argc)
	{
		(*i)++;
		return ;
	}
	var_key = get_variable_name(ast->data.cmd_node.argv, *i + 1);
	value = get_env(table, var_key);
	free(var_key);
	if (value.is_error)
		expanded = expand_var(allocs, create_success(""));
	else
		expanded = expand_var(allocs, value);
	ast->data.cmd_node.argv[*i] = expanded;
	shift_arguments(ast, *i + 1);
}

void	handle_substitution_arg(t_ast *ast, t_allocs *allocs, t_table *table,
							int *i)
{
	char	*arg;
	char	*expanded;

	arg = ast->data.cmd_node.argv[*i];
	if (arg[0] == '$' && arg[1] == '\0')
	{
		replace_vari(ast, allocs, table, i);
		return ;
	}
	if (arg[0] == '\'' && arg[ft_strlen(arg) - 1] == '\'')
	{
		(*i)++;
		return ;
	}
	expanded = expand_variable(arg, allocs, table);
	if (expanded)
		ast->data.cmd_node.argv[*i] = expanded;
	(*i)++;
}

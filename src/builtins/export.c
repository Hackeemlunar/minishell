/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sngantch <sngantch@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 20:58:04 by sngantch          #+#    #+#             */
/*   Updated: 2025/05/16 21:22:03 by sngantch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

static void	print_environment_variables(t_table *table)
{
	t_env	*node;
	int		i;

	i = -1;
	while (++i < HASH_SIZE)
	{
		node = table->bucket[i];
		while (node)
		{
			if (node->value)
				ft_printf("declare -x %s=\"%s\"\n", node->key, node->value);
			else
				ft_printf("declare -x %s\n", node->key);
			node = node->next;
		}
	}
}

static void	add_exported_variable(char *key, char *value, t_table *table)
{
	t_result	res;
	t_env		*existing;
	char		*new_value;

	res = get_env(table, key);
	if (!res.is_error)
	{
		existing = res.data.value;
		if (existing)
		{
			new_value = NULL;
			if (value)
				new_value = ft_strdup(value);
			if (value && !new_value)
				return ;
			if (existing->value)
				free(existing->value);
		}
		free(existing->value);
		existing->value = ft_strdup(value);
	}
	else
	{
		add_env(table, key, value);
	}
}

char	*extract_variable_name(char *arg, char *equal_sign)
{
	char	*key;

	if (!equal_sign)
		return (ft_strdup(arg));
	key = ft_substr(arg, 0, equal_sign - arg);
	return (key);
}

void	process_export_arg(char *arg, t_table *table, int *exit_status)
{
	char	*equal_sign;
	char	*key;

	equal_sign = ft_strchr(arg, '=');
	key = extract_variable_name(arg, equal_sign);
	if (!is_valid_variable_name(key))
	{
		ft_printf("minishell: export: `%s': not a valid identifier", arg);
		write(STDERR_FILENO, "\n", 1);
		*exit_status = 1;
		free(key);
		return ;
	}
	if (equal_sign)
		add_exported_variable(key, equal_sign + 1, table);
	else
		add_exported_variable(key, NULL, table);
	free(key);
}

void	export_command(char **args, t_table *table, int *exit_status)
{
	int	i;

	i = 1;
	if (!args[1])
	{
		print_environment_variables(table);
		return ;
	}
	while (args[i])
	{
		process_export_arg(args[i], table, exit_status);
		i++;
	}
}

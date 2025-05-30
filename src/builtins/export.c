/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 20:58:04 by sngantch          #+#    #+#             */
/*   Updated: 2025/05/24 17:22:05 by hmensah-         ###   ########.fr       */
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
	if (!key)
		return ;
	if (!is_valid_variable_name(key))
	{
		ft_printf("minishell: export: `%s': not a valid identifier", arg);
		write(STDERR_FILENO, "\n", 1);
		*exit_status = 1;
		free(key);
		return ;
	}
	if (equal_sign)
		add_env(table, key, equal_sign + 1);
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

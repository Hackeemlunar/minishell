/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 18:58:57 by hmensah-          #+#    #+#             */
/*   Updated: 2025/05/05 19:15:40 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	init_table(t_table *table)
{
	int	i;

	i = 0;
	while (i < HASH_SIZE)
	{
		table->bucket[i] = NULL;
		i++;
	}
	table->size = 0;
}

void	clean_env(t_table *table)
{
	t_env	*entry;
	t_env	*next;
	int		i;

	i = 0;
	while (i < HASH_SIZE)
	{
		entry = table->bucket[i];
		while (entry)
		{
			next = entry->next;
			free(entry->key);
			free(entry->value);
			free(entry);
			entry = next;
		}
		table->bucket[i] = NULL;
		i++;
	}
	table->size = 0;
}

/**
* @brief Initialize the environment variables by creating a hash table from the
* received environment variables from the shell.
*/
t_result	init_env(t_table *table, char **env)
{
	int		i;
	char	*key;
	char	*value;
	char	*equal;

	i = 0;
	init_table(table);
	while (env[i])
	{
		equal = ft_strchr(env[i], '=');
		if (equal)
		{
			key = ft_substr(env[i], 0, equal - env[i]);
			value = ft_substr(equal + 1, 0, ft_strlen(equal + 1));
			if (!key || !value)
				return (create_error(NO_MEMORY));
			add_env(table, key, value);
			free(key);
			free(value);
		}
		i++;
	}
	table->size = i;
	return (create_success(table));
}

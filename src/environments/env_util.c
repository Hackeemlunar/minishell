/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_util.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 18:27:57 by hmensah-          #+#    #+#             */
/*   Updated: 2025/05/22 20:12:10 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static unsigned int	hash(const char *key)
{
	unsigned int	hash;
	int				c;

	hash = 5381;
	c = *key++;
	while (c)
	{
		hash = ((hash << 5) + hash) + c;
		c = *key++;
	}
	return (hash % HASH_SIZE);
}

static inline t_result	add_new(t_table *table, int index, char *key, char *val)
{
	t_env			*new_env;

	new_env = malloc(sizeof(t_env));
	if (!new_env)
		return (create_error(NO_MEMORY));
	new_env->key = ft_strdup(key);
	new_env->value = ft_strdup(val);
	if (!new_env->key || !new_env->value)
	{
		if (new_env->key)
			free(new_env->key);
		if (new_env->value)
			free(new_env->value);
		free(new_env);
		return (create_error(NO_MEMORY));
	}
	new_env->next = table->bucket[index];
	table->bucket[index] = new_env;
	return (create_success(table));
}

// add a new environment variable to the hash table
// if the key already exists, update its value
t_result	add_env(t_table *table, char *key, char *value)
{
	unsigned int	index;
	t_env			*entry;
	t_result		result;

	if (!key || !value)
		return (create_error(INVALID_ARGUMENT));
	index = hash(key);
	entry = table->bucket[index];
	while (entry)
	{
		if (ft_strcmp(entry->key, key) == 0)
		{
			free(entry->value);
			entry->value = ft_strdup(value);
			return (create_success(table));
		}
		entry = entry->next;
	}
	result = add_new(table, index, key, value);
	if (result.is_error)
		return (result);
	return (create_success(table));
}

// get the value of an environment variable by its key
// if the key does not exist, return NULL
t_result	get_env(t_table *table, char *key)
{
	t_env			*entry;
	unsigned int	index;

	if (!key)
		return (create_error(INVALID_ARGUMENT));
	index = hash(key);
	entry = table->bucket[index];
	while (entry)
	{
		if (ft_strcmp(entry->key, key) == 0)
			return (create_success(entry->value));
		entry = entry->next;
	}
	return (create_error(NO_FILE));
}

// remove an environment variable by its key
// if the key does not exist, return NULL
// this function frees the memory allocated for the key and value
// and the entry itself
t_result	delete_env(t_table *table, char *key)
{
	t_env			*entry;
	t_env			*prev;
	unsigned int	index;

	if (!key)
		return (create_error(INVALID_ARGUMENT));
	index = hash(key);
	entry = table->bucket[index];
	prev = NULL;
	while (entry)
	{
		if (ft_strcmp(entry->key, key) == 0)
		{
			if (prev)
				prev->next = entry->next;
			else
				table->bucket[index] = entry->next;
			free(entry->key);
			free(entry->value);
			return (free(entry), create_success(table));
		}
		prev = entry;
		entry = entry->next;
	}
	return (create_error(NO_FILE));
}

#include "msh_env.h"


/**
* @brief Initialize the environment variables by creating a hash table from the
* received environment variables from the shell.
*/

unsigned int hash(const char *key)
{
	unsigned int	hash;;
	int		c;

	hash = 5381;
	while ((c = *key++))
	{
		hash = ((hash << 5) + hash) + c;
	}
	return (hash % HASH_SIZE);
}

t_result init_env(t_table *table, char **env)
{
	int		i;
	t_env	*new_env;

	table->size = 0;
	i = 0;
	while (env[i])
	{
		new_env = malloc(sizeof(t_env));
		if (!new_env)
			return (create_error(NO_MEMORY));
		new_env->key = ft_strdup(env[i]);
		if (!new_env->key)
			return (create_error(NO_MEMORY));
		new_env->value = ft_strdup(env[i]);
		if (!new_env->value)
			return (create_error(NO_MEMORY));
		table->bucket[hash(new_env->key)] = new_env;
		table->size++;
		i++;
	}
	return (create_success(table));
}

// add a new environment variable to the hash table
// if the key already exists, update its value
t_result	add_env(t_table *table, char *key, char *value)
{
	t_env		*new_env;
	unsigned int	index;

	if (!key || !value)
		return (create_error(INVALID_ARGUMENT));
	index = hash(key);
	new_env = table->bucket[index];
	while (new_env)
	{
		if (ft_strcmp(new_env->key, key) == 0)
		{
			free(new_env->value);
			new_env->value = ft_strdup(value);
			return (create_success(table));
		}
		new_env = new_env->next;
	}
	new_env = malloc(sizeof(t_env));
	if (!new_env)
		return (create_error(NO_MEMORY));
	new_env->key = ft_strdup(key);
	new_env->value = ft_strdup(value);
	if (!new_env->value || !new_env->key)
		return (create_error(NO_MEMORY));
	table->bucket[index] = new_env;
	return (create_success(table));
}

// get the value of an environment variable by its key
// if the key does not exist, return NULL
t_result	get_env(t_table *table, char *key)
{
	t_env		*entry;
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

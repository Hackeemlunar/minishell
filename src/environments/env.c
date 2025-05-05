#include "../minishell.h"


/**
* @brief Initialize the environment variables by creating a hash table from the
* received environment variables from the shell.
*/

static unsigned int	hash(const char *key)
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

// add a new environment variable to the hash table
// if the key already exists, update its value
t_result add_env(t_table *table, char *key, char *value)
{
    unsigned int index;
    t_env *new_env;

    if (!key || !value)
        return create_error(INVALID_ARGUMENT);

    index = hash(key);
    t_env *entry = table->bucket[index];

    // Check for existing key
    while (entry) {
        if (ft_strcmp(entry->key, key) == 0) {
            free(entry->value);
            entry->value = ft_strdup(value);
            return create_success(table);
        }
        entry = entry->next;
    }

    // Create new entry
    new_env = malloc(sizeof(t_env));
    if (!new_env)
        return create_error(NO_MEMORY);

    new_env->key = ft_strdup(key);
    new_env->value = ft_strdup(value);
    if (!new_env->key || !new_env->value) {
        free(new_env->key);
        free(new_env->value);
        free(new_env); // ✅ Free the node itself
        return create_error(NO_MEMORY);
    }

    new_env->next = table->bucket[index]; // ✅ Link to existing bucket
    table->bucket[index] = new_env;      // ✅ Insert at head
    return create_success(table);
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

// remove an environment variable by its key
// if the key does not exist, return NULL
// this function frees the memory allocated for the key and value
// and the entry itself
t_result	delete_env(t_table *table, char *key)
{
	t_env		*entry;
	t_env		*prev;
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

static void init_table(t_table *table)
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

void clean_env(t_table *table)
{
	t_env	*entry;
	t_env	*next;
	int	i;

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

t_result init_env(t_table *table, char **env)
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


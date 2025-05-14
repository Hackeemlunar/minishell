
#include "builtins.h"
// #include "../minishell.h"

static void	print_environment_variables(t_table *table)
{
	for (int i = 0; i < HASH_SIZE; i++)
	{
		t_env *node = table->bucket[i];
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

static void	 add_exported_variable(char *key, char *value, t_table *table)
{
	t_result res = get_env(table, key);

	if (!res.is_error)
	{
		// Update value if already exists
		// t_env *existing = res.data.value;
		t_env *existing = res.data.value;
		if (existing)
		{
			char *new_value = NULL;
			if (value)
				new_value = ft_strdup(value);
				
			if (value && !new_value)
				return; // Memory allocation failed
				
			if (existing->value)
				free(existing->value);
		}
		free(existing->value);
		existing->value = ft_strdup(value);
	}
	else
	{
		add_env(table, key, value); // Insert new
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

int	is_valid_variable_name(char *key)
{
	int	i;

	i = 0;
	if (!ft_isalpha(key[i]) && key[i] != '_')
		return (0);
	i++;
	while (key[i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

void	export_command(char **args, t_table *table, int *exit_status)
{
	int		i = 1;
	char	*equal_sign;
	char	*key;

	if (!args[1])
	{
		print_environment_variables(table);
		return;
	}

	while (args[i])
	{
		equal_sign = ft_strchr(args[i], '=');
		key = extract_variable_name(args[i], equal_sign);

		if (!is_valid_variable_name(key))
		{
			ft_printf(
				"minishell: export: `%s': not a valid identifier", args[i]);
			write(STDERR_FILENO, "\n", 1);
			*exit_status = 1;
			free(key);
			i++;
			continue;
		}
		
		if (equal_sign)
			add_exported_variable(key, equal_sign + 1, table);
		else
			add_exported_variable(key, NULL, table);

		free(key);
		i++;
	}
}

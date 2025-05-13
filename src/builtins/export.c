
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

static void	add_exported_variable(char *key, char *value, t_table *table)
{
	t_result res = get_env(table, key);
	if (!res.is_error)
	{
		// Update value if already exists
		t_env *existing = res.data.value;
		free(existing->value);
		existing->value = ft_strdup(value);
	}
	else
	{
		add_env(table, key, value); // Insert new
	}
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

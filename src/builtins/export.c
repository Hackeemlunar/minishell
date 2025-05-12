#include "builtins.h"


static void	print_environment_variables(t_env_var *env_var_list)
{
	t_env_var	*current;

	current = env_var_list;
	while (current)
	{
		if (current->value)
			ft_printf("declare -x %s=\"%s\"\n", current->key, current->value);
		else
			ft_printf("declare -x %s\n", current->key);
		current = current->next;
	}
}

static void	add_exported_variable(char *equal_sign,
	char *key, t_env_var **env_var_list)
{
	t_env_var	*new_node;

	if (!equal_sign)
	{
		new_node = env_var_new(key, NULL);
		env_var_insert_sorted(env_var_list, new_node);
	}
	else
	{
		new_node = env_var_new(key, ft_strdup(equal_sign + 1));
		env_var_insert_sorted(env_var_list, new_node);
	}
}

void	export_command(char **args,
	t_env_var **env_var_list, int *exit_status)
{
	int			i;
	char		*key;
	char		*equal_sign;

	if (!args[1])
	{
		print_environment_variables(*env_var_list);
		return ;
	}
	i = 0;
	while (args[++i])
	{
		equal_sign = ft_strchr(args[i], '=');
		key = extract_variable_name(args[i], equal_sign);
		if (!is_valid_variable_name(key))
		{
			ft_printf_fd(STDERR_FILENO,
				"minishell: export: `%s': not a valid identifier\n", args[i]);
			*exit_status = 1;
			i++;
			free(key);
			continue ;
		}
		add_exported_variable(equal_sign, key, env_var_list);
	}
}

void	export(char **args, t_env_var *env_var_list)
{
	if (!args[1])
	{
		print_environment_variables(env_var_list);
		return ;
	}
}

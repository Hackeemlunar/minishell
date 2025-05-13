#include "builtins.h"
// #include "../minishell.h"

void	env(char **argv, t_table *table)
{
	int i;
	t_env *node;

	if (argv[1])
	{
		ft_printf("env: %s: No such file or directory", argv[1]);
		write(STDERR_FILENO, "\n", 1);
		exit(127);  // Simulates Bash's behavior
	}
	i = -1;
	while (++i < HASH_SIZE)
	{
		node = table->bucket[i];
		while (node)
		{
			if (node->value)
				ft_printf("%s=%s\n", node->key, node->value);
			node = node->next;
		}
	}
}


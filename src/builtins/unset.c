#include "builtins.h"

// #include "builtins.h"
// #include "minishell.h"

void	unset(char *key, t_table *table, int *exit_status)
{
	delete_env(table, key);
	*exit_status = 0;
}

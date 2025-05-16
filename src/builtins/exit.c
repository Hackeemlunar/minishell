#include "builtins.h"

void	ft_exit(t_allocs *allocs, t_table *table)
{
	clean_mshell(allocs, table);
	ft_printf("exit\n");
	exit(1);
}

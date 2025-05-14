#include "builtins.h"

// #include "builtins.h"
// #include "minishell.h"

void	unset(char **args, t_table *table, int *exit_status)
{
	int i = 1;

	if (!table || !exit_status || !args)
		return;

	while (args[i])
	{
		if (!is_valid_variable_name(args[i]))
		{
			ft_printf("minishell: unset: `%s': not a valid identifier", args[i]);
            write(STDERR_FILENO, "\n", 1);
			*exit_status = 1;
		}
		else
		{
			t_result res = delete_env(table, args[i]);
			if (res.is_error)
			{
				// debug info
                *exit_status = 1;
			}
		}
		i++;
	}
}

#include "builtins.h"

void	env(char **argv, t_params *params)
{
	t_env_var	*tmp;

	if (argv[1])
	{
		ft_printf_fd(2, "env: %s: No such file or directory\n", argv[1]);
		free_exit(params, 127);
	}
	tmp = params->env_var_list;
	while (tmp)
	{
		if (tmp->value)
			ft_printf("%s=%s\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}
}
#include "builtins.h"



static void	handle_non_numeric_arg(char *arg, t_params *params)
{
	ft_printf("exit\n");
	ft_printf_fd(STDERR_FILENO,
		"minishell: exit: %s: numeric argument required\n", arg);
	free_exit(params, 255);
}

static void	handle_too_many_args(t_params *params)
{
	ft_printf("exit\n");
	ft_printf_fd(STDERR_FILENO, "minishell: exit: too many arguments\n");
	free_exit(params, 1);
}

void	exit_command(char **argv, t_params *params)
{
	long long	exit_code;
	int			over_under_flow;

	if (argv[1])
	{
		if (!is_numeric(argv[1]))
			handle_non_numeric_arg(argv[1], params);
		else if (argv[2])
			handle_too_many_args(params);
		else
		{
			ft_printf("exit\n");
			exit_code = ft_atoll(argv[1], &over_under_flow);
			if (over_under_flow)
			{
				ft_printf_fd(STDERR_FILENO,
					"minishell: exit: %s: numeric argument required\n", argv[1]);
				free_exit(params, 255);
			}
			free_exit(params, exit_code);
		}
	}
}

#include "builtins.h"
// #include <limits.h>
// #include <errno.h>

static int	is_numeric(const char *str)
{
	if (!str || *str == '\0')
		return (0);
	if (*str == '-' || *str == '+')
		str++;
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}

static void	exit_cleanup_and_exit(t_mshell *shell, int status)
{
	ft_printf("exit\n");
	exit(status);
}

void	ft_exit(char **argv, t_mshell *shell)
{
	long long	exit_code;
	int			overflow_flag;

	(void)shell; // if you plan to clean shell resources, remove this

	if (!argv[1])
		exit_cleanup_and_exit(shell, 0);

	if (!is_numeric(argv[1]))
	{
		ft_printf("exit\n");
		ft_dprintf(STDERR_FILENO, "minishell: exit: %s: numeric argument required\n", argv[1]);
		exit_cleanup_and_exit(shell, 255);
	}
	if (argv[2])
	{
		ft_printf("exit\n");
		ft_dprintf(STDERR_FILENO, "minishell: exit: too many arguments\n");
		// Do not exit immediately: shell stays alive (Bash behavior)
		return ;
	}
	exit_code = ft_atoll(argv[1], &overflow_flag);
	if (overflow_flag)
	{
		ft_dprintf(STDERR_FILENO, "minishell: exit: %s: numeric argument required\n", argv[1]);
		exit_cleanup_and_exit(shell, 255);
	}
	exit_cleanup_and_exit(shell, (unsigned char)exit_code);
}

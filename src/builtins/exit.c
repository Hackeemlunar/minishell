#include "builtins.h"
// #include <limits.h>
// #include <errno.h>

static int	check_overflow(long long num, int sign, int digit)
{
	if (num > LONG_MAX / 10 || (num == LONG_MAX / 10 && digit > LLONG_MAX % 10))
	{
		if (sign == 1 || (sign == -1 && (digit > 8)))
			return (1);
	}
	return (0);
}

long long	ft_atoll(const char *str, int *over_under_flow)
{
	int			i;
	int			sign;
	long long	num;

	i = 0;
	sign = 1;
	num = 0;
	while (str[i] && ((str[i] >= 9 && str[i] <= 13) || str[i] == 32))
		i++;
	if (str[i] && (str[i] == '+' || str[i] == '-'))
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] && (str[i] >= '0' && str[i] <= '9'))
	{
		if (check_overflow(num, sign, str[i] - '0'))
			return (*over_under_flow = 1, 0);
		num = (num * 10) + (str[i] - '0');
		i++;
	}
	return (*over_under_flow = 0, num * sign);
}


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
	(void)shell;
	ft_printf("exit\n");
	exit(status);
}

void	ft_exit(char **argv, t_mshell *shell)
{
	long long	exit_code;
	int			overflow_flag;

	if (!shell)
	{
		ft_printf("exit\n");
		exit(1);
	}

	if (!argv[1])
		exit_cleanup_and_exit(shell, 0);

	if (!is_numeric(argv[1]))
	{
		ft_printf("exit\n minishell: exit: %s: numeric argument required", argv[1]);
		write(STDERR_FILENO, "\n", 1);
		exit_cleanup_and_exit(shell, 255);
	}
	if (argv[2])
	{
		ft_printf("exit\n");
		write(STDERR_FILENO, "minishell: exit: too many arguments\n", 36);
		// Do not exit immediately: shell stays alive (Bash behavior)
		return ;
	}
	exit_code = ft_atoll(argv[1], &overflow_flag);
	if (overflow_flag)
	{
		ft_printf("minishell: exit: %s: numeric argument required\n", argv[1]);
		write(STDERR_FILENO, "\n", 1);
		exit_cleanup_and_exit(shell, 255);
	}
	exit_cleanup_and_exit(shell, (unsigned char)exit_code);
}

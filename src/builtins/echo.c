#include "builtins.h"

static int	is_newline_flag(char *str)
{
	int	i;

	i = 1;
	if (str[0] == '-')
	{
		while (str[i] && str[i] == 'n')
			i++;
		if (str[i] == '\0')
			return (1);
	}
	return (0);
}

void	echo(char **argv)
{
	int		i;
	int		new_line;

	i = 1;
	new_line = 1;
	while (argv[i] && is_newline_flag(argv[i]))
	{
		new_line = 0;
		i++;
	}
	while (argv[i])
	{
		printf("%s", argv[i]);
		if (argv[i++ + 1])
			printf(" ");
	}
	if (new_line)
		printf("\n");
}

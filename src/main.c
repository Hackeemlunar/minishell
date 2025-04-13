#include "minishell.h"

void	show_banner(void)
{
	printf("\n"
		"|************************** MiniShell v1.0 ***********************|\n"
		"|*                                                               *|\n"
		"|*                   Welcome to the MiniShell!                   *|\n"
		"|*                                                               *|\n"
		"|*                Created by \033[31m%s and %s\033[0m              "
		" *|\n"
		"|*                                                               *|\n"
		"|*               Type 'help' for a list of commands.             *|\n"
		"|*                                                               *|\n"
		"|*                     Type 'exit' to quit.                      *|\n"
		"|*                                                               *|\n"
		"|*                                                               *|\n"
		"|*                           Born2Code                           *|\n"
		"|*****************************************************************|\n"
		"\n", "Sngantch", "Hmensah-");
}

int	main(void)
{
	char	*str;
	char	**cmd;

	show_banner();
	str = readline("minishell> ");
	if (str == NULL)
	{
		printf("Error: readline failed\n");
		return (1);
	}
	cmd = parse_cmd(str);
	if (cmd == NULL)
	{
		printf("Error: parse_cmd failed\n");
		free(str);
		return (1);
	}
	int i = 0;
	while (cmd[i])
	{
		printf("Command %d: %s\n", i, cmd[i]);
		free(cmd[i]);
		i++;
	}
	free(cmd);
	free(str);
	return (0);
}

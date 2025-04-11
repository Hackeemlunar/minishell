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

	show_banner();
	str = readline("minishell> ");
	if (str == NULL)
	{
		printf("Error: readline failed\n");
		return (1);
	}
	printf("You entered: %s\n", str);
	free(str);
	return (0);
}

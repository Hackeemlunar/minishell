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

void	init_allocators(t_allocs *allocs)
{
	allocs->parse_alloc = arena_create(4096);
}

int	main(void)
{
	char		*str;
	t_result	result;
	t_allocs	allocs;
	t_mshell	mshell;

	show_banner();
	init_allocators(&allocs);
	str = readline("minishell> ");
	if (str == NULL)
	{
		printf("Error: readline failed\n");
		return (1);
	}
	result = parse_cmdln(str, &mshell, &allocs);
	printf("%d\n", result.data.error);
	arena_destroy(allocs.parse_alloc);
	free(str);
	return (0);
}

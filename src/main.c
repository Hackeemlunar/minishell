/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:33:25 by hmensah-          #+#    #+#             */
/*   Updated: 2025/05/16 16:48:15 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

static void	command_loop(t_mshell *shell, t_allocs *allocs, t_table *table)
{
	char		*str;
	t_result	result;

	while (true)
	{
		setup_signals();
		str = readline("$mshell-> ");
		if (!str)
			return ;
		if (check_all_white_space(str))
		{
			free(str);
			continue ;
		}
		add_history(str);
		result = parse_cmdln(str, shell, allocs);
		if (result.is_error)
		{
			print_parse_error(result);
			free(str);
			continue ;
		}
		run_command(shell, allocs, table, result);
		free(str);
	}
}



int main(int argc, char **argv, char **envp)
{
	t_allocs	allocs;
	t_mshell	mshell;
	t_table		env_table;
	t_result	result;

	(void)argc;
	(void)argv;
	show_banner();
	init_allocators(&allocs);
	read_history("./histfile");
	if (init_env(&env_table, envp).is_error)
		return (1);
	mshell.env = envp;
	result = get_paths(&env_table, &mshell.paths, &allocs);
	if (result.is_error)
		return (1);
	command_loop(&mshell, &allocs, &env_table);
	write(STDOUT_FILENO, "exit\n", 5);
	write_history("./histfile");
	clean_mshell(&allocs, &env_table);
	return (mshell.exit_status);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sngantch <sngantch@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:33:25 by hmensah-          #+#    #+#             */
/*   Updated: 2025/05/25 21:31:15 by sngantch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_in_child = 0;

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

void	print_parse_error(t_result result)
{
	if (result.data.error == INVALID_PIPE)
		ft_putendl_fd("minishell: invalid pipe", 2);
	else if (result.data.error == INVALID_QUOTE)
		ft_putendl_fd("minishell: invalid quote", 2);
	else if (result.data.error == INVALID_REDIRECT)
		ft_putendl_fd("minishell: invalid redirect", 2);
	else if (result.data.error == INVALID_VAR)
		ft_putendl_fd("minishell: invalid variable", 2);
	else if (result.data.error == INVALID_SYNTAX)
		ft_putendl_fd("minishell: syntax error", 2);
	else
		ft_putendl_fd("minishell: unknown error", 2);
}

static void	command_loop(t_mshell *shell, t_allocs *allocs, t_table *table)
{
	char		*str;
	t_result	result;

	while (true)
	{
		rl_already_prompted = 0;
		str = readline("$minishell-> ");
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

int	main(int argc, char **argv, char **envp)
{
	t_allocs	allocs;
	t_mshell	mshell;
	t_table		env_table;

	(void)argc;
	(void)argv;
	show_banner();
	init_allocators(&allocs);
	setup_signals();
	read_history("./histfile");
	if (init_env(&env_table, envp).is_error)
		return (1);
	mshell.env = envp;
	mshell.exit_status = 0;
	command_loop(&mshell, &allocs, &env_table);
	write(STDOUT_FILENO, "exit\n", 5);
	write_history("./histfile");
	clean_mshell(&allocs, &env_table);
	return (mshell.exit_status);
}

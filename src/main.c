/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sngantch <sngantch@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:33:25 by hmensah-          #+#    #+#             */
/*   Updated: 2025/05/13 14:15:35 by sngantch         ###   ########.fr       */
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

void	init_allocators(t_allocs *allocs)
{
	allocs->parse_alloc = arena_create(4096);
	allocs->sh_alloc = arena_create(4096);
	allocs->exec_alloc = arena_create(4096);
}

void	clean_allocators(t_allocs *allocs)
{
	arena_destroy(allocs->parse_alloc);
	arena_destroy(allocs->sh_alloc);
	arena_destroy(allocs->exec_alloc);
}

static t_result get_paths(t_table *table, char ***paths, t_allocs *allocs)
{
    t_result    result;
    char        *path;
    char        **path_split;
    int         i;

    result = get_env(table, "PATH");
    if (result.is_error)
        return (result);
    path = result.data.value;
    path_split = ft_split(path, ':');
    if (!path_split)
        return (create_error(NO_MEMORY));
    i = 0;
    while (path_split[i])
        i++;
    *paths = arena_alloc(allocs->sh_alloc, (i + 1) * sizeof(char*));
    i = -1;
    while (path_split[++i])
    {
        (*paths)[i] = arena_alloc(allocs->sh_alloc, ft_strlen(path_split[i]) + 3);
        ft_strlcpy((*paths)[i], path_split[i], ft_strlen(path_split[i]) + 1);
        ft_strlcat((*paths)[i], "/", ft_strlen(path_split[i]) + 2);
        free(path_split[i]);
    }
    (*paths)[i] = NULL;
    return (free(path_split), create_success(NULL));
}

int	check_all_white_space(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!whitespace(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int main(int argc, char **argv, char **envp)
{
	char		*str;
	t_result	result;
	t_allocs	allocs;
	t_mshell	mshell;
	t_table		env_table;

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
	setup_signals();
	while (true)
	{
		
		str = readline("sh$mshell-> ");
		if (!str)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			break;
		}
		if (check_all_white_space(str))
		{
			free(str);
			continue ;
		}
		add_history(str);
		result = parse_cmdln(str, &mshell, &allocs);
		if (result.is_error) {
			ft_printf("%d", result.data.error);
			write(STDOUT_FILENO, "\n", 1);
			continue ;
		}
		mshell.ast = result.data.value;
		result = run_command(&mshell, &allocs, &env_table);
		if (result.is_error) {
			printf("%d", result.data.error);
			write(STDOUT_FILENO, "\n", 1);
			continue ;
		}
		free(str);
	}
	write_history("./histfile");
	clean_env(&env_table);
	clean_allocators(&allocs);
	free(str);
	return (0);
}

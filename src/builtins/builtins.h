#ifndef BUILTINS_H
#define BUILTINS_H

#include "../minishell.h"

void	cd(char **argv, t_table *table, int *exit_status);
void	echo(char **argv);
void	env(char **argv, t_table *table);
void	export_command(char **args, t_table *table, int *exit_status);
void	unset(char *key, t_table *table, int *exit_status);
void	pwd(int *exit_status);
void	ft_exit(char **argv, t_mshell *shell, t_allocs *allocs);

#endif


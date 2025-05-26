/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sngantch <sngantch@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 21:03:36 by sngantch          #+#    #+#             */
/*   Updated: 2025/05/26 15:41:34 by sngantch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "../minishell.h"

void	cd(char **argv, t_table *table, int *exit_status);
int     echo(t_ast *node);
void	env(char **argv, t_table *table);
void	export_command(char **args, t_table *table, int *exit_status);
void	unset(char *key, t_table *table, int *exit_status);
void	pwd(int *exit_status);
// void	ft_exit(t_allocs *allocs, t_table *table);
void	ft_exit(t_allocs *allocs, t_table *table, t_ast *node, t_mshell *shell);
int		is_valid_variable_name(char *key);
void    history(int *exit_status);
#endif

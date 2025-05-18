/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sngantch <sngantch@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 21:03:36 by sngantch          #+#    #+#             */
/*   Updated: 2025/05/16 21:20:56 by sngantch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "../minishell.h"

void	cd(char **argv, t_table *table, int *exit_status);
void	echo(char **argv);
void	env(char **argv, t_table *table);
void	export_command(char **args, t_table *table, int *exit_status);
void	unset(char *key, t_table *table, int *exit_status);
void	pwd(int *exit_status);
void	ft_exit(t_allocs *allocs, t_table *table);
int		is_valid_variable_name(char *key);
#endif

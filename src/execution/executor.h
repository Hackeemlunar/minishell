/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 18:55:23 by hmensah-          #+#    #+#             */
/*   Updated: 2025/05/07 19:14:13 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

#include "../minishell.h"

void	walk_ast(t_ast *ast, t_mshell *shell, t_allocs *allocs, t_table *table);
char	*expand_variable(char *str, t_allocs *allocs, t_table *table);
int	collect_heredoc_input(const char *delim, char *temp_file);
int	space_or_quote(char c);
void	expand_substitutions(t_ast *ast, t_allocs *allocs, t_table *table);
#endif

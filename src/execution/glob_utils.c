/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 00:00:00 by hmensah-          #+#    #+#             */
/*   Updated: 2025/01/27 00:00:00 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	copy_matches_to_argv(char **new_argv, char **matches,
						int start_idx, int match_cnt)
{
	int	j;

	j = 0;
	while (j < match_cnt)
	{
		new_argv[start_idx + j] = matches[j];
		j++;
	}
}

void	copy_args_after_wildcard(char **new_argv, char **old_argv,
							int idx, int match_cnt)
{
	int	i;
	int	old_argc;

	old_argc = 0;
	while (old_argv[old_argc])
		old_argc++;
	i = idx + 1;
	while (i < old_argc)
	{
		new_argv[i - 1 + match_cnt] = old_argv[i];
		i++;
	}
}

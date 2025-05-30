/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 20:58:15 by sngantch          #+#    #+#             */
/*   Updated: 2025/05/30 16:16:37 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

void	pwd(int *exit_status)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (pwd)
	{
		ft_printf("%s\n", pwd);
		free(pwd);
	}
	else
	{
		perror("pwd");
		*exit_status = 1;
	}
}

int	setup_builtin_redirections(t_in_out *io, t_allocs *alloc,
	t_table *table, int *fds)
{
	fds[0] = dup(STDOUT_FILENO);
	fds[1] = dup(STDIN_FILENO);
	if (io)
	{
		if (set_in_fds(io, alloc, table))
		{
			dup2(fds[0], STDOUT_FILENO);
			dup2(fds[1], STDIN_FILENO);
			close(fds[0]);
			close(fds[1]);
			return (1);
		}
		if (set_out_fds(io, alloc, table))
		{
			dup2(fds[0], STDOUT_FILENO);
			dup2(fds[1], STDIN_FILENO);
			close(fds[0]);
			close(fds[1]);
			return (1);
		}
	}
	return (0);
}

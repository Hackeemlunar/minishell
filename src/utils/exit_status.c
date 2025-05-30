/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_status.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sngantch <sngantch@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 10:00:00 by hmensah-          #+#    #+#             */
/*   Updated: 2025/05/26 14:50:12 by sngantch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * Set the exit status in the shell structure
 */
void	set_exit_status(t_mshell *shell, int status)
{
	if (shell)
		shell->exit_status = status & 0xFF;
}

/**
 * Get the current exit status from the shell structure
 */
int	get_exit_status(t_mshell *shell)
{
	if (shell)
		return (shell->exit_status);
	return (0);
}

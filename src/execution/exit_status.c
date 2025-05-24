/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_status.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 10:00:00 by hmensah-          #+#    #+#             */
/*   Updated: 2025/05/15 15:44:55 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

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

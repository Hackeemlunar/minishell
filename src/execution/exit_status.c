/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_status.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 10:00:00 by hmensah-          #+#    #+#             */
/*   Updated: 2025/05/15 14:08:41 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

/**
 * Set the exit status in the shell structure
 */
void	set_exit_status(t_mshell *shell, int status)
{
	if (shell)
		shell->exit_status = status;
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

/**
 * Update exit status based on the command result
 */
void	update_exit_status(t_mshell *shell, t_result result)
{
	if (!shell)
		return ;
	if (result.is_error)
	{
		if (result.data.error == NO_PATH || result.data.error == INVALID_CMD)
			shell->exit_status = 127;
		else if (result.data.error == NO_FILE)
			shell->exit_status = 1;
		else
			shell->exit_status = 1;
	}
	else
		shell->exit_status = 0;
}

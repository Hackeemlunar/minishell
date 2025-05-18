/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sngantch <sngantch@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 20:58:20 by sngantch          #+#    #+#             */
/*   Updated: 2025/05/16 20:58:21 by sngantch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

void	unset(char *key, t_table *table, int *exit_status)
{
	delete_env(table, key);
	*exit_status = 0;
}

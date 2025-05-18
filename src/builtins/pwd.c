/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sngantch <sngantch@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 20:58:15 by sngantch          #+#    #+#             */
/*   Updated: 2025/05/16 21:06:25 by sngantch         ###   ########.fr       */
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

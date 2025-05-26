/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sngantch <sngantch@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 11:18:41 by sngantch          #+#    #+#             */
/*   Updated: 2025/05/26 11:32:01 by sngantch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

void    history(int *exit_status)
{
    int fd;
    char *line;

    fd = open("../.././histfile", O_RDONLY);
    if (fd < 0)
    {
        perror("minishell: history");
        if (exit_status)
            *exit_status = 1;
        else
            ft_putendl_fd("minishell: history: could not open history file", STDERR_FILENO);
        return ;
        
    }

    line = get_next_line(fd);
    while (line != NULL)
    {
        ft_putstr_fd(line, STDOUT_FILENO);
        free(line);
        line = get_next_line(fd);
    }
    close(fd);
    if (exit_status)
        *exit_status = 0;
}
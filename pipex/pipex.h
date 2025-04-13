/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 18:05:55 by hmensah-          #+#    #+#             */
/*   Updated: 2025/03/22 20:21:40 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H
# include <errno.h>
# include <sys/wait.h>
# include <sys/types.h>
# include "../libft/libft.h"

typedef struct s_cmdline
{
	char	*cmd;
	char	**cmd_args;
	int		in_fd;
	int		out_fd;
}	t_cmdline;

char	**parse_cmd(char const *s);
void	extract_path(t_cmdline *cmd, char **env);
void	run_command(t_cmdline *cmd, int in_fd, int out_fd, char **env);
void	safe_dup2(int old_fd, int new_fd);
void	setup_cmd(t_cmdline *cmd, int fd, char *full_cmd, int is_input);
#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 18:05:55 by hmensah-          #+#    #+#             */
/*   Updated: 2025/03/22 20:22:12 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H
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

typedef struct s_pipex
{
	int		num_cmds;
	int		heredoc;
	int		in_out_fd[2];
	pid_t	*pids;
}	t_pipex;

char	**parse_cmd(char const *s);
int		check_heredoc(char *heredoc);
int		get_heredoc_fd_setup(int *in_out_fd, char **argv, int num_cmds);
pid_t	run_command(t_cmdline *cmd, int in_fd, int out_fd, char **env);
void	cleanup_cmd(t_cmdline *cmds);
void	write_until_limiter(char *limiter);
void	extract_path(t_cmdline *cmd, char **env);
void	setup_fd(int *fd, char *file, int flags, mode_t mode);
void	safe_pipe(int *pipe_fd);

#endif

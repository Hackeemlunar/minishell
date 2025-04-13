/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus_util2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 18:38:54 by hmensah-          #+#    #+#             */
/*   Updated: 2025/03/22 17:59:54 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static void	safe_dup2(int old_fd, int new_fd)
{
	if (dup2(old_fd, new_fd) < 0)
	{
		perror("dup2");
		exit(EXIT_FAILURE);
	}
	close(old_fd);
}

void	safe_pipe(int *pipe_fd)
{
	if (pipe(pipe_fd) < 0)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
}

pid_t	run_command(t_cmdline *cmd, int in_fd, int out_fd, char **env)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		safe_dup2(in_fd, STDIN_FILENO);
		safe_dup2(out_fd, STDOUT_FILENO);
		execve(cmd->cmd, cmd->cmd_args, env);
		perror("execve");
		cleanup_cmd(cmd);
		exit(EXIT_FAILURE);
	}
	return (pid);
}

int	check_heredoc(char *heredoc)
{
	int	heredoc_len;

	heredoc_len = 8;
	if (ft_strncmp("here_doc", heredoc, heredoc_len) == 0)
		return (1);
	return (0);
}

int	get_heredoc_fd_setup(int *in_out_fd, char **argv, int num_cmds)
{
	int	heredoc;
	int	flags;

	if (check_heredoc(argv[1]))
		heredoc = 1;
	else
		heredoc = 0;
	if (heredoc)
	{
		flags = O_WRONLY | O_CREAT | O_APPEND;
		setup_fd(&in_out_fd[0], "/tmp/heredoc", O_RDONLY, 0);
		setup_fd(&in_out_fd[1], argv[num_cmds + 3], flags, 0644);
	}
	else
	{
		flags = O_WRONLY | O_CREAT | O_TRUNC;
		setup_fd(&in_out_fd[0], argv[1], O_RDONLY, 0);
		setup_fd(&in_out_fd[1], argv[num_cmds + 2], flags, 0644);
	}
	return (heredoc);
}

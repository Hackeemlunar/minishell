/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus_util1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 18:39:00 by hmensah-          #+#    #+#             */
/*   Updated: 2025/02/27 18:44:45 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	cleanup_cmd(t_cmdline *cmds)
{
	char	**full_cmds;

	if (cmds->cmd_args)
	{
		full_cmds = cmds->cmd_args;
		while (*full_cmds)
		{
			free(*full_cmds);
			full_cmds++;
		}
		free(cmds->cmd_args);
		cmds->cmd_args = NULL;
	}
	if (access("/tmp/heredoc", F_OK) == 0)
		unlink("/tmp/heredoc");
}

void	write_until_limiter(char *limiter)
{
	char	*line;
	int		limiter_len;
	int		fd;

	limiter_len = ft_strlen(limiter);
	fd = open("/tmp/heredoc", O_WRONLY | O_CREAT, 0644);
	line = get_next_line(0);
	while (line != NULL)
	{
		if (ft_strncmp(limiter, line, limiter_len) == 0)
		{
			free(line);
			break ;
		}
		ft_putstr_fd(line, fd);
		free(line);
		line = get_next_line(0);
	}
	close(fd);
}

static void	extract_path_dir(t_cmdline *cmd, char **dirs)
{
	int		i;
	char	*cmd_with_path;
	char	*temp;

	i = -1;
	while (dirs[++i] != NULL)
	{
		temp = ft_strjoin(dirs[i], "/");
		cmd_with_path = ft_strjoin(temp, cmd->cmd);
		free(temp);
		if (access(cmd_with_path, F_OK) == 0)
		{
			free(cmd->cmd);
			cmd->cmd = cmd_with_path;
			cmd->cmd_args[0] = cmd_with_path;
			break ;
		}
		free(cmd_with_path);
	}
	i = 0;
	while (dirs[i])
		free(dirs[i++]);
	free(dirs);
}

void	extract_path(t_cmdline *cmd, char **env)
{
	char	*path;
	char	**dirs;
	int		i;

	i = 0;
	path = NULL;
	while (env[i] != NULL)
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
		{
			path = env[i] + 5;
			break ;
		}
		i++;
	}
	if (!path)
		return ;
	dirs = ft_split(path, ':');
	if (!dirs)
		return ;
	extract_path_dir(cmd, dirs);
}

void	setup_fd(int *fd, char *file, int flags, mode_t mode)
{
	*fd = open(file, flags, mode);
	if (*fd < 0)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}
}

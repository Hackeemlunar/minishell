/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 14:23:58 by hmensah-          #+#    #+#             */
/*   Updated: 2025/03/18 21:08:03 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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
		}
		else
			free(cmd_with_path);
		free(dirs[i]);
	}
}

void	extract_path(t_cmdline *cmd, char **env)
{
	char	*path;
	char	**dirs;
	int		i;

	i = -1;
	while (env[++i] != NULL)
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
		{
			path = env[i] + 5;
			break ;
		}
	}
	dirs = ft_split(path, ':');
	extract_path_dir(cmd, dirs);
	free(dirs);
	dirs = NULL;
}

void	setup_cmd(t_cmdline *cmd, int fd, char *full_cmd, int is_input)
{
	ft_memset(cmd, 0, sizeof(t_cmdline));
	cmd->cmd_args = parse_cmd(full_cmd);
	if (!cmd->cmd_args)
	{
		ft_putstr_fd("Command parsing failed\n", 2);
		exit(EXIT_FAILURE);
	}
	cmd->cmd = cmd->cmd_args[0];
	if (is_input)
		cmd->in_fd = fd;
	else
		cmd->out_fd = fd;
}

void	safe_dup2(int old_fd, int new_fd)
{
	if (dup2(old_fd, new_fd) < 0)
	{
		perror("dup2");
		exit(EXIT_FAILURE);
	}
}

void	run_command(t_cmdline *cmd, int in_fd, int out_fd, char **env)
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
		exit(EXIT_FAILURE);
	}
}

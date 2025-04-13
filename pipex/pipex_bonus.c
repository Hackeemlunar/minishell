/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 17:06:18 by hmensah-          #+#    #+#             */
/*   Updated: 2025/03/22 17:59:39 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	setup_cmd(t_cmdline *cmd, int fd, char *full_cmd, int is_input)
{
	ft_memset(cmd, 0, sizeof(t_cmdline));
	cmd->cmd_args = parse_cmd(full_cmd);
	if (!cmd->cmd_args)
	{
		ft_putstr_fd("Command parsing failed\n", 2);
		cleanup_cmd(cmd);
		exit(EXIT_FAILURE);
	}
	cmd->cmd = cmd->cmd_args[0];
	if (is_input)
		cmd->in_fd = fd;
	else
		cmd->out_fd = fd;
}

static void	execute_cmd(t_cmdline *cmds, t_pipex *p, char **argv, char **env)
{
	int		pipes[2];
	int		i;

	i = 0;
	while (i < p->num_cmds)
	{
		if (i < p->num_cmds - 1)
			safe_pipe(pipes);
		setup_cmd(&cmds[i], p->in_out_fd[0], argv[i + 2 + p->heredoc], 1);
		extract_path(&cmds[i], env);
		if (i == p->num_cmds - 1)
			p->pids[i] = run_command(&cmds[i], cmds[i].in_fd,
					p->in_out_fd[1], env);
		else
			p->pids[i] = run_command(&cmds[i], cmds[i].in_fd, pipes[1], env);
		cleanup_cmd(&cmds[i]);
		close(p->in_out_fd[0]);
		if (i < p->num_cmds - 1)
		{
			close(pipes[1]);
			p->in_out_fd[0] = pipes[0];
		}
		i++;
	}
}

static void	wait_and_cleanup(t_pipex *p)
{
	int	i;

	i = 0;
	while (i < p->num_cmds)
	{
		waitpid(p->pids[i], NULL, 0);
		i++;
	}
	free(p->pids);
	close(p->in_out_fd[1]);
	close(p->in_out_fd[0]);
}

int	execute_pipeline(t_cmdline *cmds, char **argv, char **env, int num_cmds)
{
	t_pipex	p;

	p.num_cmds = num_cmds;
	p.heredoc = get_heredoc_fd_setup(p.in_out_fd, argv, num_cmds);
	p.pids = malloc(sizeof(pid_t) * num_cmds);
	if (!p.pids)
		exit(EXIT_FAILURE);
	execute_cmd(cmds, &p, argv, env);
	wait_and_cleanup(&p);
	return (0);
}

int	main(int argc, char **argv, char **env)
{
	t_cmdline	cmds[20];
	int			num_cmds;

	if (argc > 5 && check_heredoc(argv[1]))
	{
		num_cmds = argc - 4;
		write_until_limiter(argv[2]);
	}
	else if (argc > 4 && !check_heredoc(argv[1]))
		num_cmds = argc - 3;
	else
	{
		ft_putendl_fd("Usg: ./pipex infile cmd1 .. cmdn outfile", 2);
		ft_putendl_fd("Usg: ./pipex here_doc LIMITER cmd1 .. cmdn outfile", 2);
		return (1);
	}
	execute_pipeline(cmds, argv, env, num_cmds);
	return (0);
}

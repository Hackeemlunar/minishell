/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sngantch <sngantch@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 21:00:30 by sngantch          #+#    #+#             */
/*   Updated: 2025/05/27 18:25:49 by sngantch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_signals.h"

void	signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		if (!g_in_child)
		{
			write(STDERR_FILENO, "\n", 1);
			rl_replace_line("", 0);
			rl_on_new_line();
			rl_redisplay();
		}
		else
			write(STDERR_FILENO, "\n", 1);
	}
	else if (signum == SIGQUIT)
	{
		if (g_in_child)
		{
			ft_printf("Quit: %d\b", SIGQUIT);
			write(STDERR_FILENO, "\n", 1);
		}
	}
}

void	signal_handler_heredoc(int signum)
{
	if (signum == SIGINT)
	{
		rl_replace_line("", 0);
		rl_on_new_line();
		exit(1);
	}
}

void	signal_handler_input(int signum)
{
	if (signum == SIGINT)
	{
		write(STDERR_FILENO, "\n", 1);
	}
	else if (signum == SIGQUIT)
	{
		ft_printf("Quit: %d\b", SIGQUIT);
		write(STDERR_FILENO, "\n", 1);
	}
}

void	setup_signals(void)
{
	disable_echoctl();
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, signal_handler);
	signal(SIGTSTP, signal_handler);
}

void	set_signal_handler(t_ast *ast)
{
	if (ast->data.cmd_node.io && ast->data.cmd_node.io->heredoc_delim)
	{
		signal(SIGINT, signal_handler_heredoc);
		signal(SIGQUIT, SIG_IGN);
	}
	else
	{
		signal(SIGINT, signal_handler_input);
		signal(SIGQUIT, signal_handler_input);
	}
}

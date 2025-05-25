/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 21:00:30 by sngantch          #+#    #+#             */
/*   Updated: 2025/05/25 16:44:40 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_signals.h"

void disable_echoctl(void)
{
    struct termios term;

    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~ECHOCTL;
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

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
	else if (signum == SIGQUIT || signum == SIGTSTP)
	{
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
	else if (signum == SIGQUIT)
	{
		write(STDOUT_FILENO, "\b\b  \b\b", 5);
	}
}

void	signal_handler_input(int signum)
{
	if (signum == SIGQUIT)
	{
		ft_printf("Quit: %d\b", SIGQUIT);
		write(STDERR_FILENO, "\n", 1);
	}
	else if (signum == SIGINT)
	{
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
		signal(SIGQUIT, signal_handler_heredoc);
	}
	else
	{
		signal(SIGINT, signal_handler_input);
		signal(SIGQUIT, signal_handler_input);
	}
}

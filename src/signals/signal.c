/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sngantch <sngantch@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 21:00:30 by sngantch          #+#    #+#             */
/*   Updated: 2025/05/18 18:13:16 by sngantch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_signals.h"

void    signal_handler(int signum)
{
    if (signum == SIGINT)
    {
        write(STDERR_FILENO, "\n", 1);
        rl_already_prompted = 1;
        rl_replace_line("", 0);
        rl_on_new_line();
        rl_redisplay();
    }
    else if (signum == SIGQUIT || signum == SIGTSTP)
    {
    }
}

void    signal_handler_heredoc(int signum)
{
    int fd;
    int child_pid;

    if (signum == SIGINT)
    {
        fd = open("/tmp/child_pid.tmp", O_RDONLY);
        if (fd < 0)
        {
            perror("open");
            exit(1);
        }
        if (read(fd, &child_pid, sizeof(int)) < 0)
        {
            perror("read");
            exit(1);
        }
        close(fd);
        kill(child_pid, SIGINT);
        rl_already_prompted = 1;
        exit(1);
    }
    else if (signum == SIGQUIT)
    {
        write(STDOUT_FILENO, "\b\b  \b\b", 5);
    }
}


void signal_handler_input(int signum)
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

void    setup_signals(void)
{
    struct sigaction sa = {0};
    
    sa.sa_handler = signal_handler;
    sa.sa_flags = SA_RESTART;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);
    
    sa.sa_handler = SIG_IGN;
    sa.sa_flags = 0; // Reset sa_flags
    sigaction(SIGQUIT, &sa, NULL);
    sigaction(SIGTSTP, &sa, NULL);
}

void set_signal_handler(t_ast *ast)
{
    struct sigaction sa = {0};
    
    sa.sa_flags = SA_RESTART;
    sigemptyset(&sa.sa_mask);
    if (ast->data.cmd_node.io && ast->data.cmd_node.io->heredoc_delim)
    {
        sa.sa_handler = signal_handler_heredoc;
    }
    else
    {
        sa.sa_handler = signal_handler_input;
    }
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
}

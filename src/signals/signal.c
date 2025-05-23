
#include "msh_signals.h"

void signal_handler(int signum)
{
    if (signum == SIGINT)
    {
        write(STDERR_FILENO, "\n", 1);
        rl_replace_line("", 0);
        rl_on_new_line();
        rl_redisplay();
    }
    else if (signum == SIGQUIT)
    {
    }
}

void signal_handler_heredoc(int signum)
{
    int fd;
    int child_pid;

    if (signum == SIGINT)
    {
        write(STDOUT_FILENO, "\n", 1);
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


void setup_signals(void)
{
    struct sigaction sa = {0};

    sa.sa_handler = signal_handler;
    sa.sa_flags = SA_RESTART;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);
    signal(SIGQUIT, SIG_IGN);
    
}


void set_signal_handler(t_ast *node)
{
    if (node && node->type == NODE_CMD
        && node->data.cmd_node.io
        && node->data.cmd_node.io->heredoc_delim != NULL)
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

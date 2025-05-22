#include "msh_signals.h"

void disable_echoctl(void)
{
    struct termios term;

    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~ECHOCTL;
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

// Main signal handler
void signal_handler(int signum)
{
    if (signum == SIGINT)
    {
        rl_cleanup_after_signal();
        write(STDERR_FILENO, "\n", 1);
        rl_replace_line("", 0);
        rl_on_new_line();
        rl_reset_after_signal();
        rl_redisplay();
    }
}

// Heredoc handler
void signal_handler_heredoc(int signum)
{
    if (signum == SIGINT)
    {
        rl_replace_line("", 0);
        rl_on_new_line();
        exit(1);
    }
}

// Child process handler
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
    struct sigaction    sa;
    
    disable_echoctl();
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
    sigaction(SIGTSTP, &sa, NULL);
}

void set_signal_handler(t_ast *ast)
{
    struct sigaction sa;
    
    if (ast->data.cmd_node.io && ast->data.cmd_node.io->heredoc_delim)
    {
        sa.sa_handler = signal_handler_heredoc;
        sa.sa_flags = SA_RESTART;
        sigemptyset(&sa.sa_mask);
        sigaction(SIGINT, &sa, NULL);
        sigaction(SIGQUIT, &sa, NULL);
    }
    else
    {
        sa.sa_handler = signal_handler_input;
        sa.sa_flags = SA_RESTART;
        sigemptyset(&sa.sa_mask);
        sigaction(SIGINT, &sa, NULL);
        sigaction(SIGQUIT, &sa, NULL);
    }
}

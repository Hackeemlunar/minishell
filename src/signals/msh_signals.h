#ifndef MSH_SIGNALS_H
#define MSH_SIGNALS_H

#define CMD_REDIR    1
#define CMD_HEREDOC  2
#define CMD_EXEC     0


#include <signal.h>     // sigaction, sigemptyset, kill, SIGKILL, etc.
#include "../minishell.h"  // ← Gives access to t_cmd and command types

// Setup general shell signal handlers (for prompt and execution)
void setup_signals(void);
// Set signal handlers based on command type
void	set_signal_handler(t_cmd *tree);
// Special signal handler for heredoc interruption (SIGINT)
void signal_handler_heredoc(int signum);

#endif

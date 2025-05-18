// #include <signal.h>
// #include <readline/readline.h>

// // ...existing code...

// void	signal_handler(int sig)
// {
// 	if (sig == SIGINT)
// 	{
// 		rl_replace_line("", 0); // Clear the current input line
// 		rl_on_new_line();       // Move to a new line
// 		rl_redisplay();         // Redisplay the prompt
// 	}
// 	else if (sig == SIGQUIT)
// 	{
// 		// Ignore SIGQUIT to prevent core dumps
// 		write(1, "\b\b  \b\b", 6);
// 	}
// }

// void	heredoc_signal_handler(int sig)
// {
// 	if (sig == SIGINT)
// 	{
// 		write(1, "\n", 1); // Print a newline
// 		exit(1);           // Exit the heredoc process
// 	}
// }

// void	set_signal_handler(void (*handler)(int))
// {
// 	if (!handler)
// 		return;
// 	signal(SIGINT, handler);
// 	signal(SIGQUIT, SIG_IGN); // Ignore SIGQUIT
// }

// void	set_heredoc_signal_handler(void)
// {
// 	signal(SIGINT, heredoc_signal_handler);
// 	signal(SIGQUIT, SIG_IGN); // Ignore SIGQUIT
// }

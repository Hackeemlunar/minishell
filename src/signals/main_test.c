#include "msh_signals.h"

void simulate_heredoc_child()
{
	int pid = fork();

	if (pid == 0)
	{
		// Child process
		signal(SIGINT, SIG_DFL);  // Let Ctrl+C kill child
		signal(SIGQUIT, SIG_IGN); // Ignore Ctrl+\

		char *line;
		while (1)
		{
			line = readline("heredoc> ");
			if (!line)
			{
				printf("EOF received. Exiting heredoc.\n");
				exit(0);
			}
			free(line);
		}
	}
	else
	{
		waitpid(pid, NULL, 0);
		// Reset terminal after heredoc
		rl_reset_after_signal();
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}


void run_fake_child_command()
{
	int pid = fork();
	if (pid == 0)
	{
		printf("Child running... press Ctrl+C or Ctrl+\\ to test signals.\n");
		while (1)
			sleep(1);
	}
	else
	{
		waitpid(pid, NULL, 0);
	}
}

int main(void)
{
	char *input;

	while (1)
	{
		setup_signals();  // Default signal mode (prompt)
		input = readline("minishell> ");

		if (!input)
		{
			printf("exit\n");
			break;
		}
		else if (strcmp(input, "exit") == 0)
		{
			free(input);
			break;
		}
		else if (strcmp(input, "heredoc") == 0)
		{
			t_cmd tmp = {.type = CMD_REDIR};
			set_signal_handler(&tmp); // heredoc mode
			simulate_heredoc_child();
		}
		else if (strcmp(input, "child") == 0)
		{
			t_cmd tmp = {.type = CMD_EXEC};
			set_signal_handler(&tmp); // exec mode
			run_fake_child_command();
		}
		else
		{
			printf("Unrecognized input: %s\n", input);
		}
		free(input);
	}
	return 0;
}

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

void run_real_command(char *input)
{
	pid_t pid = fork();

	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);

		char *argv[256];
		char *token = strtok(input, " ");
		int i = 0;
		while (token && i < 255)
		{
			argv[i++] = token;
			token = strtok(NULL, " ");
		}
		argv[i] = NULL;

		if (execvp(argv[0], argv) == -1)
		{
			perror("minishell");
			exit(EXIT_FAILURE);
		}
	}
	else if (pid > 0)
	{
		int status;
		waitpid(pid, &status, 0);
	}
	else
	{
		perror("fork");
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
			set_signal_handler(&tmp);
			simulate_heredoc_child();
		}
		else
		{
			t_cmd tmp = {.type = CMD_EXEC};
			set_signal_handler(&tmp);
			run_real_command(input);
		}
		free(input);
	}
	return 0;
}
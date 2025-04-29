#ifndef MINISHELL_H
# define MINISHELL_H
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <unistd.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <errno.h>
# include <term.h>
# include "../include/readline.h"
# include "../include/history.h"
# include "../libft/libft.h"
# include "../libft/arena.h"

/**
 * @struct s_allocators
 * @brief A structure that holds memory arenas for different
 * components of the program.
 *
 * This structure is used to manage memory allocation for various
 * parts of the program such as shell, builtins, execution, parsing,
 * environment, and signal handling.
 *
 * @var s_allocators::sh_alloc
 * Memory arena for the shell.
 * @var s_allocators::builtins_alloc
 * Memory arena for built-in commands.
 * @var s_allocators::exec_alloc
 * Memory arena for execution-related data.
 * @var s_allocators::parsing_alloc
 * Memory arena for parsing-related data.
 * @var s_allocators::env_alloc
 * Memory arena for environment variables.
 * @var s_allocators::sig_alloc
 * Memory arena for signal handling.
 */
typedef struct s_allocators
{
	t_arena		*sh_alloc;
	t_arena		*builtins_alloc;
	t_arena		*exec_alloc;
	t_arena		*parse_alloc;
	t_arena		*env_alloc;
	t_arena		*sig_alloc;
}			t_allocs;

/**
 * @enum e_error
 * @brief Enumeration of error codes used throughout the program.
 *
 * This enumeration defines various error codes that represent specific
 * error conditions encountered during the execution of the program.
 *
 * @var e_error::NO_ERROR
 * No error occurred.
 * @var e_error::ERROR
 * Generic error.
 * @var e_error::NO_MEMORY
 * Memory allocation failed.
 * @var e_error::NO_FILE
 * File not found.
 * @var e_error::NO_DIR
 * Directory not found.
 * @var e_error::INVALID_CMD
 * Invalid command.
 * @var e_error::INVALID_PIPE
 * Invalid pipe usage.
 * @var e_error::INVALID_QUOTE
 * Invalid quotation usage.
 * @var e_error::INVALID_REDIRECT
 * Invalid redirection usage.
 * @var e_error::INVALID_VAR
 * Invalid variable usage.
 */
typedef enum e_error
{
	NO_ERROR = 0,
	ERROR = -1,
	NO_MEMORY = -2,
	NO_FILE = -3,
	NO_DIR = -4,
	INVALID_CMD = -5,
	INVALID_PIPE = -6,
	INVALID_QUOTE = -7,
	INVALID_REDIRECT = -8,
	INVALID_VAR = -9
}			t_error;

/**
 * @union u_data
 * @brief A union to store either an error code or a generic pointer value.
 *
 * This union is used to represent data that can either be an error code
 * or a pointer to some value, depending on the context.
 *
 * @var u_data::error
 * Error code of type t_error.
 * @var u_data::value
 * Generic pointer to a value.
 */
typedef union u_data
{
	t_error		error;
	void		*value;
}			t_data;

/**
 * @struct s_result
 * @brief A structure to encapsulate a result with error handling.
 *
 * This structure is used to represent the result of an operation, which
 * can either be a valid value or an error. It includes a flag to indicate
 * whether the result is an error.
 */
typedef struct s_result
{
	t_data		data;
	bool		is_error;
}			t_result;

/**
 * @struct s_cmd
 * @brief A structure to represent a command and its associated data.
 *
 * This structure is used to store information about a command, including
 * the command string, its arguments, and file descriptors for input and output.
 */
typedef struct s_cmd
{
	char	*cmd;
	char	**cmd_args;
	int		in_fd;
	int		out_fd;
}	t_cmd;

/**
 * @struct s_in_out
 * @brief Represents input and output file information and descriptors.
 *
 * This structure is used to manage input and output file paths, file
 * descriptors, and related settings for a shell or command execution context.
 *
 * @var s_in_out::in_file
 * Path to the input file. If NULL, no input redirection is specified.
 *
 * @var s_in_out::out_file
 * Path to the output file. If NULL, no output redirection is specified.
 *
 * @var s_in_out::heredoc
 * Indicates whether a heredoc is used. Non-zero value means heredoc is enabled.
 *
 * @var s_in_out::in_fd
 * File descriptor for the input file. Defaults to -1 if not set.
 *
 * @var s_in_out::out_fd
 * File descriptor for the output file. Defaults to -1 if not set.
 *
 * @var s_in_out::out_mode
 * Mode for opening the output file (e.g., append or truncate).
 */
typedef struct s_in_out
{
	char	*in_file;
	char	*out_file;
	char	*heredoc_delim;
	int		in_fd;
	int		in_mode;
	int		out_fd;
	int		out_mode;
}	t_in_out;

typedef struct s_minishell
{
	pid_t		*pids;
	t_cmd		*cmds;
	t_in_out	*in_out;
	int			num_cmds;
}	t_mshell;

t_result	create_success(void *value);
t_result	create_error(t_error error_code);
t_result	lex_cmdln(const char *cmdline, t_mshell *shell, t_allocs *allocs);
t_result	parse_cmdln(t_result *lex_cmdln, t_mshell *shell, t_allocs *allocs);
#endif
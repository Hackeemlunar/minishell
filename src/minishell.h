/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 13:58:53 by hmensah-          #+#    #+#             */
/*   Updated: 2025/05/22 18:11:07 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
# include <termios.h>
# include "../include/readline/readline.h"
# include "../include/readline/history.h"
# include "../libft/libft.h"
# include "../libft/arena.h"

# define HASH_SIZE 128

typedef struct s_env
{
	char		*key;
	char		*value;
	struct s_env	*next;
}			t_env;

typedef struct s_table
{
	t_env	*bucket[HASH_SIZE];
	int		size;
}	t_table;

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
	INVALID_VAR = -9,
	INVALID_SYNTAX = -10,
	INVALID_ARGUMENT = -11,
	PID_ERROR = -12,
	INVALID_ENV = -13,
	NO_PATH = -14
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

/**
 * @enum e_node_type
 * @brief Enumeration of node types in the Abstract Syntax Tree (AST).
 *
 * This enumeration defines various types of nodes that can exist in the
 * Abstract Syntax Tree (AST) used for parsing shell commands.
 *
 * @var e_node_type::NODE_CMD
 * Represents a command node.
 * @var e_node_type::NODE_PIPELINE
 * Represents a pipeline node.
 * @var e_node_type::NODE_AND
 * Represents an AND operation node.
 * @var e_node_type::NODE_OR
 * Represents an OR operation node.
 * @var e_node_type::NODE_BACKGROUND
 * Represents a background execution node.
 * @var e_node_type::NODE_SUBSHELL
 * Represents a subshell node.
 */
typedef enum e_node_type
{
	NODE_CMD,
	NODE_PIPELINE,
	NODE_AND,
	NODE_OR,
	NODE_BACKGROUND,
	NODE_SUBSHELL
}	t_node_type;

/**
* @struct s_ast
* @brief Abstract Syntax Tree (AST) node structure.
*
* This structure represents a node in the Abstract Syntax Tree (AST)
* used for parsing shell commands. It can represent different types
* of nodes, including command nodes, binary operation nodes,
* and subshell nodes.
*/
typedef struct s_ast
{
	t_node_type			type;
	union u_data_ast
	{
		struct s_cmd_node
		{
			int			argc;
			char		**argv;
			t_in_out	*io;
		}	cmd_node;
		struct s_bin_node
		{
			struct s_ast	*left;
			struct s_ast	*right;
		}	bin_op_node;
		struct s_ast		*sub;
	}		data;
}			t_ast;

typedef struct s_minishell
{
	pid_t		*pids;
	int			pid_track;
	t_ast		*ast;
	char		**env;
	char		**paths;
	int			num_cmds;
	int			exit_status;
}	t_mshell;

bool		is_special_char(char c);
void		init_allocators(t_allocs *allocs);
void		clean_mshell(t_allocs *allocs, t_table *table);
int			check_all_white_space(char *str);
t_result 	get_paths(t_table *table, char ***paths, t_allocs *allocs);
t_result	create_success(void *value);
t_result	create_error(t_error error_code);
t_result	parse_cmdln(char *cmdln, t_mshell *shell, t_allocs *allocs);
t_result	add_env(t_table *table, char *key, char *value);
t_result	get_env(t_table *table, char *key);
t_result	init_env(t_table *table, char **env);
t_result	delete_env(t_table *table, char *key);
void		clean_env(t_table *table);
void		clean_mshell(t_allocs *allocs, t_table *table);
void		setup_signals(void);
void		set_signal_handler(t_ast *tree);
void 		signal_handler_heredoc(int signum);
void		setup_signals(void);
int			run_command(t_mshell *shell, t_allocs *allocs, t_table *table,
						t_result result);
int			handle_builtins(char **argv, t_mshell *shell,
						t_table *table, t_allocs *alloc);
#endif

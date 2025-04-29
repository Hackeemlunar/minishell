/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 18:05:45 by hmensah-          #+#    #+#             */
/*   Updated: 2025/04/29 15:35:48 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

/**
 * Count the number of commands (pipe segments) in the token list
 */
static int	count_commands(t_token *tokens)
{
	int		count;
	t_token	*current;

	count = 1;  // At least one command
	current = tokens;

	while (current && current->type != TOKEN_EOF)
	{
		if (current->type == TOKEN_PIPE)
			count++;
		current = current->next;
	}

	return (count);
}

/**
 * Count the number of arguments in a command segment
 */
static int	count_args(t_token *start, t_token **end)
{
	int		count;
	t_token	*current;

	count = 0;
	current = start;

	while (current && current->type != TOKEN_PIPE && current->type != TOKEN_EOF)
	{
		if (current->type == TOKEN_WORD)
			count++;
		else if (current->type == TOKEN_REDIR_IN ||
				current->type == TOKEN_REDIR_OUT ||
				current->type == TOKEN_APPEND ||
				current->type == TOKEN_HEREDOC)
		{
			// Skip the redirection token and its argument
			if (current->next && current->next->type == TOKEN_WORD)
				current = current->next;
			else
				break; // Invalid syntax, missing redirection target
		}
		current = current->next;
	}

	*end = current;
	return (count);
}

/**
 * Parse redirections in a command segment
 */
static t_error	parse_redirections(t_token *start, t_token *end,
								t_in_out *in_out)
{
	t_token	*current;

	current = start;
	while (current != end && current && current->type != TOKEN_PIPE &&
			current->type != TOKEN_EOF)
	{
		if (current->type == TOKEN_REDIR_IN)
		{
			if (current->next && current->next->type == TOKEN_WORD)
			{
				in_out->in_file = current->next->value;
				in_out->in_mode = 0; // Normal input
				current = current->next; // Skip the filename
			}
			else
				return (INVALID_REDIRECT);
		}
		else if (current->type == TOKEN_REDIR_OUT)
		{
			if (current->next && current->next->type == TOKEN_WORD)
			{
				in_out->out_file = current->next->value;
				in_out->out_mode = 0; // Truncate
				current = current->next; // Skip the filename
			}
			else
				return (INVALID_REDIRECT);
		}
		else if (current->type == TOKEN_APPEND)
		{
			if (current->next && current->next->type == TOKEN_WORD)
			{
				in_out->out_file = current->next->value;
				in_out->out_mode = 1; // Append
				current = current->next; // Skip the filename
			}
			else
				return (INVALID_REDIRECT);
		}
		else if (current->type == TOKEN_HEREDOC)
		{
			if (current->next && current->next->type == TOKEN_WORD)
			{
				in_out->heredoc_delim = current->next->value;
				in_out->in_mode = 1; // Heredoc
				current = current->next; // Skip the delimiter
			}
			else
				return (INVALID_REDIRECT);
		}
		current = current->next;
	}

	return (NO_ERROR);
}

/**
 * Parse arguments in a command segment
 */
static t_error	parse_arguments(t_token *start, t_token *end, t_cmd *cmd)
{
	t_token	*current;
	int		arg_idx;

	arg_idx = 0;
	current = start;

	while (current != end && current && current->type != TOKEN_PIPE &&
			current->type != TOKEN_EOF)
	{
		if (current->type == TOKEN_WORD)
		{
			// First word is the command
			if (arg_idx == 0)
				cmd->cmd = current->value;

			// All words are stored as arguments (including command)
			cmd->cmd_args[arg_idx++] = current->value;
		}
		else if (current->type == TOKEN_REDIR_IN ||
				current->type == TOKEN_REDIR_OUT ||
				current->type == TOKEN_APPEND ||
				current->type == TOKEN_HEREDOC)
		{
			// Skip redirection token and its argument
			if (current->next && current->next->type == TOKEN_WORD)
				current = current->next;
		}
		current = current->next;
	}

	// NULL-terminate the argument list
	cmd->cmd_args[arg_idx] = NULL;

	return (NO_ERROR);
}

/**
 * Initialize a command structure
 */
static void	init_cmd(t_cmd *cmd, int arg_count, t_arena *alloc)
{
	cmd->cmd = NULL;
	cmd->cmd_args = (char **)arena_alloc(alloc, sizeof(char *) * (arg_count + 1));
	cmd->in_fd = STDIN_FILENO;
	cmd->out_fd = STDOUT_FILENO;
}

/**
 * Initialize input/output structure
 */
static void	init_in_out(t_in_out *in_out)
{
	in_out->in_file = NULL;
	in_out->out_file = NULL;
	in_out->heredoc_delim = NULL;
	in_out->in_fd = -1;
	in_out->in_mode = 0;  // Normal input
	in_out->out_fd = -1;
	in_out->out_mode = 0; // Normal output (truncate)
}

/**
 * Parse the token stream into a shell command structure
 */
t_result	parse_cmdln(t_result *lex_result, t_mshell *shell, t_allocs *allocs)
{
	t_token	*tokens;
	t_token	*current;
	t_token	*end;
	int		cmd_idx;
	int		arg_count;
	t_error	error;

	if (!lex_result || lex_result->is_error || !shell || !allocs->parse_alloc)
		return (create_error(ERROR));

	tokens = (t_token *)lex_result->data.value;
	if (!tokens)
		return (create_error(ERROR));

	// Count the number of commands separated by pipes
	shell->num_cmds = count_commands(tokens);

	// Allocate memory for commands and in_out structures
	shell->cmds = (t_cmd *)arena_alloc(allocs->parse_alloc,
									sizeof(t_cmd) * shell->num_cmds);
	shell->in_out = (t_in_out *)arena_alloc(allocs->parse_alloc,
									sizeof(t_in_out) * shell->num_cmds);
	shell->pids = (pid_t *)arena_alloc(allocs->parse_alloc,
									sizeof(pid_t) * shell->num_cmds);

	if (!shell->cmds || !shell->in_out || !shell->pids)
		return (create_error(NO_MEMORY));

	// Parse each command
	cmd_idx = 0;
	current = tokens;

	while (current && current->type != TOKEN_EOF)
	{
		// Initialize the current command and I/O structures
		end = NULL;
		arg_count = count_args(current, &end);
		init_cmd(&shell->cmds[cmd_idx], arg_count, allocs->parse_alloc);
		init_in_out(&shell->in_out[cmd_idx]);

		// Parse redirections first (they might affect argument count)
		error = parse_redirections(current, end, &shell->in_out[cmd_idx]);
		if (error != NO_ERROR)
			return (create_error(error));

		// Parse command and arguments
		error = parse_arguments(current, end, &shell->cmds[cmd_idx]);
		if (error != NO_ERROR)
			return (create_error(error));

		// Move to the next command after the pipe
		if (end && end->type == TOKEN_PIPE)
			current = end->next;
		else
			current = end;

		cmd_idx++;
	}

	return (create_success(shell));
}

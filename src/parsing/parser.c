/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 18:05:45 by hmensah-          #+#    #+#             */
/*   Updated: 2025/02/21 19:48:30 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static int	seen_opn(int i, char c)
{
	if (c == '\'' || c == '\"')
	{
		return (!i);
	}
	return (i);
}

static size_t	count_cmds(char const *s)
{
	size_t	count;
	int		seen_open;

	count = 1;
	seen_open = 0;
	while (*s)
	{
		seen_open = seen_opn(seen_open, *s);
		if (*s == '|' && !seen_open)
			count++;
		s++;
	}
	printf("commands = %zu\n", count);
	return (count);
}

static size_t	count_words(char const *s)
{
	size_t	count;
	int		in_word;
	int		seen_open;

	count = 0;
	in_word = 0;
	seen_open = 0;
	while (*s)
	{
		seen_open = seen_opn(seen_open, *s);
		if (*s != ' ' && in_word == 0 && !seen_open)
		{
			in_word = 1;
			count++;
		}
		else if (*s == ' ' && !seen_open)
			in_word = 0;
		s++;
	}
	return (count);
}

static char	*word_dup(const char *s, size_t start, size_t finish)
{
	char	*word;
	size_t	i;

	word = (char *)malloc((finish - start + 1) * sizeof(char));
	if (!word)
		return (NULL);
	i = 0;
	while (start < finish)
		word[i++] = s[start++];
	word[i] = '\0';
	return (word);
}

static char	**extract(char const *s, char **word)
{
	size_t	i;
	size_t	j;
	size_t	len;
	int		index;
	int		seen_open;

	index = -1;
	i = -1;
	j = 0;
	seen_open = 0;
	len = ft_strlen(s);
	while (++i <= len)
	{
		seen_open = seen_opn(seen_open, s[i]);
		if ((s[i] != ' ' || seen_open) && index < 0)
			index = i;
		else if (((s[i] == ' ' && !seen_open) || i == len) && index >= 0)
		{
			word[j++] = word_dup(s, index, i);
			index = -1;
		}
	}
	return (word);
}

char	**parse_cmd(char const *s)
{
	char	**split;

	if (!s)
		return (NULL);
	split = (char **)malloc((count_words(s) + 1) * sizeof(char *));
	if (!split)
		return (NULL);
	extract(s, split);
	return (split);
}

char	*get_word(t_allocs *alloc, char *cmdln)
{
	int		i;
	int		seen_open;
	char	*word;

	i = 0;
	seen_open = 0;
	while (cmdln[i] && cmdln[i] != ' ' && !seen_open)
	{
		seen_open = seen_opn(seen_open, cmdln[i]);
		if (cmdln[i] == '|' && !seen_open)
			break ;
		i++;
	}
	word = (char *)arena_alloc(alloc->parse_alloc, (i + 1) * sizeof(char));
	if (!word)
		return (NULL);
	ft_strlcpy(word, cmdln, i + 1);
	return (word);
}

void	init_in_file(char *cmdln, t_mshell *shell, t_allocs *alloc, int len)
{
	int	i;
	int	seen_open;

	i = 0;
	seen_open = 0;
	while (cmdln[i])
	{
		seen_open = seen_opn(seen_open, cmdln[i]);
		if (cmdln[i] == '|' && !seen_open)
			shell->in_out->in_mode = -1;
		else if (cmdln[i] == '<' && !seen_open)
		{
			if (i + 1 < len && cmdln[i + 1] == '<')
			{
				shell->in_out->heredoc_delim = get_word(alloc, cmdln + i + 2);
				shell->in_out->in_mode = 1;
			}
			else
			{
				shell->in_out->in_mode = 0;
				shell->in_out->in_file = get_word(alloc, cmdln + i + 1);
			}
		}
		i++;
	}
}

void	init_out_file(char *cmdln, t_mshell *shell, t_allocs *alloc, int len)
{
	int	i;
	int	seen_open;

	i = len - 1;
	seen_open = 0;
	while (cmdln[i])
	{
		seen_open = seen_opn(seen_open, cmdln[i]);
		if (cmdln[i] == '|' && !seen_open)
			shell->in_out->out_mode = -1;
		else if (cmdln[i] == '>' && !seen_open)
		{
			if (i - 1 > 0 && cmdln[i - 1] == '>')
				shell->in_out->out_mode = 1;
			else
				shell->in_out->out_mode = 0;
			shell->in_out->in_file = get_word(alloc, cmdln + i + 1);
		}
		i--;
	}
}

void	init_in_out(char *cmdln, t_mshell *shell, t_allocs *alloc, int num_cmds)
{
	int	len;
	int	i;
	int	seen_open;

	i = 0;
	seen_open = 0;
	len = ft_strlen(cmdln);
	init_in_file(cmdln, shell, alloc, len);
	init_out_file(cmdln, shell, alloc, len);
}

void	skip_spaces_next_word(char *cmdln, int *i)
{
	while (cmdln[*i] && cmdln[*i] == ' ')
		(*i)++;
	while (cmdln[*i] && cmdln[*i] != ' ')
		(*i)++;
}

void	count_cmdargs(char *cmdln)
{
	int		i;
	int		seen_open;
	int		word_count;
	int		in_word;

	i = -1;
	seen_open = 0;
	word_count = 0;
	in_word = 0;
	while (cmdln[++i])
	{
		seen_open = seen_opn(seen_open, cmdln[i]);
		if ((cmdln[i] == '>' || cmdln[i] == '<') && !seen_open)
		{
			i++;
			if (cmdln[i] == '>')
				skip_spaces_next_word(cmdln, &i);
			else if (cmdln[i] == '<')
				skip_spaces_next_word(cmdln, &i);
			else
				skip_spaces_next_word(cmdln, &i);
		}
		else if (!seen_open && cmdln[i] != ' ' && cmdln[i] != '|')
		{
			if (in_word == 0)
			{
				in_word = 1;
				word_count++;
			}
			else if (cmdln[i] == ' ' && !seen_open)
				in_word = 0;
		}
	}
}

void	extract_cmds(char *cmdln, t_mshell *shell, t_allocs *alloc)
{
	int		i;
	int		seen_open;
	char	*word;

	i = 0;
	seen_open = 0;
	while (cmdln[i])
	{
		seen_open = seen_opn(seen_open, cmdln[i]);
		if ((cmdln[i] == '>' || cmdln[i] == '<') && !seen_open)
		{
			i++;
			if (cmdln[i] == '>')
				skip_spaces_next_word(cmdln, &i);
			else if (cmdln[i] == '<')
				skip_spaces_next_word(cmdln, &i);
			else
				skip_spaces_next_word(cmdln, &i);
		}

	}
}

void	parse_cmdline(const char *cmdline, t_mshell *shell, t_allocs *allocs)
{
	if (!cmdline || !shell || !allocs)
	{
		perror("Error: Invalid arguments to parse_cmdline");
		exit(EXIT_FAILURE);
	}
	shell->num_cmds = count_cmds(cmdline);
	shell->cmds = (t_cmd *)arena_alloc(allocs->parse_alloc, shell->num_cmds
			* sizeof(t_cmd));
	if (!shell->cmds)
	{
		perror("Error allocating memory for commands");
		exit(EXIT_FAILURE);
	}
	shell->in_out = (t_in_out *)arena_alloc(allocs->parse_alloc,
			shell->num_cmds * sizeof(t_in_out));
	if (!shell->in_out)
	{
		perror("Error allocating memory for input/output");
		exit(EXIT_FAILURE);
	}
	init_in_out(cmdline, shell, allocs, shell->num_cmds);
	extract_cmds(cmdline, shell, allocs);
}
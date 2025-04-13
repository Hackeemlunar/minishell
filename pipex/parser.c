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

#include "pipex.h"

static int	seen_opn(int i, char c)
{
	if (c == '\'' || c == '\"')
	{
		return (!i);
	}
	return (i);
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

static char	**extract(char const *s, char **split)
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
			split[j++] = word_dup(s, index, i);
			index = -1;
		}
	}
	return (split[j] = NULL, split);
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

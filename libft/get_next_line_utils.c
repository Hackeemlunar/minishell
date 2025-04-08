/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 18:11:55 by hmensah-          #+#    #+#             */
/*   Updated: 2025/02/15 17:52:34 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_strncpy(char *dst, const char *src, size_t n)
{
	char		*d;
	const char	*s;

	d = dst;
	s = src;
	while (n > 0 && *s)
	{
		*d++ = *s++;
		n--;
	}
	while (n > 0)
	{
		*d++ = '\0';
		n--;
	}
	return (dst);
}

void	cleanup_context(t_context *ctx)
{
	ctx->buf_cap = 0;
	ctx->buf_pos = 0;
	ctx->buf_pos_prv = 0;
	ctx->stash_len = 0;
	ctx->stash_st = 0;
	ctx->nl = 0;
	ctx->err = 0;
}

char	*handle_eof_err(t_context *ctx, ssize_t byt_read)
{
	char	*line;

	if (byt_read < 0 || (ctx->stash_len + ctx->buf_pos == 0))
	{
		cleanup_context(ctx);
		return (NULL);
	}
	line = malloc(ctx->stash_len + ctx->buf_pos + 1);
	if (!line)
		return (NULL);
	if (ctx->stash_len)
		ft_strncpy(line, ctx->stash, ctx->stash_len);
	if (ctx->buf_pos)
		ft_strncpy(line + ctx->stash_len, ctx->buffer, ctx->buf_pos);
	line[ctx->stash_len + ctx->buf_pos] = '\0';
	cleanup_context(ctx);
	return (line);
}

void	handle_stash(t_context *ctx, char **line)
{
	size_t	i;

	i = 0;
	while (i < ctx->stash_len)
	{
		if (ctx->stash[i] == '\n')
		{
			i++;
			ctx->nl = 1;
			*line = malloc(i + 1);
			if (!*line)
				return ;
			ft_strncpy(*line, ctx->stash, i);
			(*line)[i] = '\0';
			ft_strncpy(ctx->stash, ctx->stash + i, ctx->stash_len - i);
			ctx->stash_len -= i;
			return ;
		}
		i++;
	}
}

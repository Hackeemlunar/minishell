/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 18:11:28 by hmensah-          #+#    #+#             */
/*   Updated: 2025/02/15 17:52:41 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	create_context(t_context *ctx)
{
	ctx->err = 0;
	ctx->nl = 0;
}

void	fill_line(t_context *ctx, char *line)
{
	size_t	remainder;

	remainder = ctx->buf_pos - ctx->buf_pos_prv;
	if (ctx->stash_len)
		ft_strncpy(line, ctx->stash, ctx->stash_len);
	ft_strncpy(line + ctx->stash_len, ctx->buffer, ctx->buf_pos_prv);
	line[ctx->stash_len + ctx->buf_pos_prv] = '\0';
	if (remainder)
	{
		ft_strncpy(ctx->stash, ctx->buffer + ctx->buf_pos_prv, remainder);
		ctx->stash_len = remainder;
	}
	else
		ctx->stash_len = 0;
	ctx->buf_pos_prv = 0;
	ctx->buf_pos = 0;
}

void	handle_line(t_context *ctx, ssize_t byt_read, char **line)
{
	ctx->buf_pos += byt_read;
	while (ctx->buf_pos_prv < ctx->buf_pos)
	{
		if (ctx->buffer[ctx->buf_pos_prv] == '\n')
		{
			ctx->buf_pos_prv++;
			ctx->nl = 1;
			*line = malloc(ctx->stash_len + ctx->buf_pos_prv + 1);
			if (!*line)
				return ;
			fill_line(ctx, *line);
			return ;
		}
		ctx->buf_pos_prv++;
	}
}

char	*get_next_line(int fd)
{
	static t_context	ctx;
	ssize_t				byt_read;
	char				*line;

	line = NULL;
	if (fd < 0 || BUFFER_SIZE <= 0 || BUFFER_SIZE > LONG_MAX)
		return (NULL);
	create_context(&ctx);
	while (!ctx.err && !ctx.nl)
	{
		if (ctx.stash_len)
		{
			handle_stash(&ctx, &line);
			if (line)
				return (line);
		}
		byt_read = read(fd, ctx.buffer + ctx.buf_pos, BUFFER_SIZE);
		if (byt_read <= 0)
			return (handle_eof_err(&ctx, byt_read));
		handle_line(&ctx, byt_read, &line);
	}
	if (ctx.err)
		cleanup_context(&ctx);
	return (line);
}

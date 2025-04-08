/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_arena.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 19:15:28 by hmensah-          #+#    #+#             */
/*   Updated: 2025/03/01 19:16:24 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_arena	*arena_create(size_t size)
{
	t_arena	*arena;

	arena = (t_arena *)malloc(sizeof(t_arena));
	if (!arena)
		return (NULL);
	arena->buffer = (char *)malloc(size);
	if (!arena->buffer)
	{
		free(arena);
		return (NULL);
	}
	arena->size = size;
	arena->used = 0;
	return (arena);
}

void	*arena_alloc(t_arena *arena, size_t size)
{
	void	*ptr;

	if (arena->used + size > arena->size)
		return (NULL);
	ptr = arena->buffer + arena->used;
	arena->used += size;
	return (ptr);
}

void	arena_reset(t_arena *arena)
{
	arena->used = 0;
}

void	arena_destroy(t_arena *arena)
{
	if (arena)
	{
		free(arena->buffer);
		free(arena);
	}
}

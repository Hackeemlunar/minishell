/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_arena.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 19:15:28 by hmensah-          #+#    #+#             */
/*   Updated: 2025/05/30 17:12:09 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "arena.h"

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
	size_t	alignment;
	size_t	current_aligned_used;
	void	*ptr;

	alignment = sizeof(void *);
	current_aligned_used = (arena->used + alignment - 1) & ~(alignment - 1);
	if (current_aligned_used + size > arena->size)
		return (NULL);
	ptr = arena->buffer + current_aligned_used;
	arena->used = current_aligned_used + size;
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

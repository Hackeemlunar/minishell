/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arena.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 17:27:23 by hmensah-          #+#    #+#             */
/*   Updated: 2025/05/30 17:28:19 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ARENA_H
# define ARENA_H

# include "libft.h"

/**
 * @brief Memory arena structure for efficient memory allocation
 *
 * This structure represents a memory arena that allows for efficient
 * allocation of memory blocks without the overhead of individual malloc/free
 * operations. The entire arena can be freed at once.
 */
typedef struct s_arena
{
	size_t	size;
	size_t	used;
	char	*buffer;
}			t_arena;

/**
 * @brief Create a new memory arena
 *
 * Allocates a new memory arena with the specified size.
 *
 * @param size Size of the arena in bytes
 * @return Pointer to the newly created arena, or NULL on failure
 */
t_arena	*arena_create(size_t size);

/**
 * @brief Reset a memory arena
 *
 * Resets the arena to its initial state, effectively freeing all allocations
 * made from it without deallocating the arena itself.
 *
 * @param arena Pointer to the arena to reset
 */
void	arena_reset(t_arena *arena);

/**
 * @brief Allocate memory from an arena
 *
 * Allocates a block of memory from the arena. The memory remains valid
 * until the arena is reset or destroyed.
 *
 * @param arena Pointer to the arena to allocate from
 * @param size Size of the memory block to allocate
 * @return Pointer to the allocated memory, or NULL if the arena is full
 */
void	*arena_alloc(t_arena *arena, size_t size);

/**
 * @brief Destroy a memory arena
 *
 * Frees all memory associated with the arena,
 * including the arena structure itself.
 *
 * @param arena Pointer to the arena to destroy
 */
void	arena_destroy(t_arena *arena);

#endif
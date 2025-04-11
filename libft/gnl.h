﻿/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gnl_private.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 00:00:00 by hmensah-          #+#    #+#             */
/*   Updated: 2025/04/11 00:00:00 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GNL_PRIVATE_H
# define GNL_PRIVATE_H

# include "libft.h"

/**
 * @brief Context structure for get_next_line function.
 *
 * This structure holds the internal state needed by get_next_line
 * to read and process files line by line.
 */
typedef struct s_context
{
	int		err;            /**< Error flag */
	int		nl;             /**< Newline indicator */
	size_t	buf_cap;        /**< Buffer capacity */
	size_t	buf_pos;        /**< Current position in buffer */
	size_t	buf_pos_prv;    /**< Previous position in buffer */
	size_t	stash_len;      /**< Length of stashed data */
	size_t	stash_st;       /**< Start position in stash */
	char	stash[BUFFER_SIZE]; /**< Temporary storage for partial reads */
	char	buffer[4096];   /**< Main buffer for constructing lines */
}			t_context;

/**
 * @brief Cleans up the context after use
 *
 * Resets the context structure to prepare it for the next read operation
 * or to clean up when done.
 *
 * @param ctx Pointer to the context structure
 */
void	cleanup_context(t_context *ctx);

/**
 * @brief Copy at most n bytes from src to dst
 *
 * A helper function used internally by get_next_line to copy strings
 *
 * @param dst Destination string
 * @param src Source string
 * @param n Maximum number of bytes to copy
 * @return Pointer to the destination string
 */
void	*ft_strncpy(char *dst, const char *src, size_t n);

/**
 * @brief Handles end of file or error conditions
 *
 * Special case handler for when we reach EOF or encounter an error while reading
 *
 * @param ctx The context structure
 * @param byt_read Number of bytes read (or -1 for error)
 * @return Resulting line or NULL in case of error
 */
char	*handle_eof_err(t_context *ctx, ssize_t byt_read);

/**
 * @brief Processes stashed data between calls to get_next_line
 *
 * Manages the leftover data from previous reads that may contain
 * the beginning of the next line.
 *
 * @param ctx The context structure
 * @param line Pointer to the line being constructed
 */
void	handle_stash(t_context *ctx, char **line);

#endif
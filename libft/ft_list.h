/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_list.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 17:26:14 by hmensah-          #+#    #+#             */
/*   Updated: 2025/05/30 17:26:44 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LIST_H
# define FT_LIST_H

# include "libft.h"

/**
 * @brief Linked list node structure
 *
 * Basic linked list node that can store any type of content
 * through a void pointer.
 */
typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

/**
 * @brief Create a new list node
 *
 * Allocates and initializes a new list node with the given content.
 *
 * @param content Pointer to the content to be stored in the new node
 * @return Pointer to the new node, or NULL if allocation fails
 */
t_list	*ft_lstnew(void *content);

/**
 * @brief Add a new node at the beginning of the list
 *
 * @param lst Address of a pointer to the first node in the list
 * @param new Pointer to the node to be added
 */
void	ft_lstadd_front(t_list **lst, t_list *new_node);

/**
 * @brief Count the number of nodes in a list
 *
 * @param lst Pointer to the first node in the list
 * @return Number of nodes in the list
 */
int		ft_lstsize(t_list *lst);

/**
 * @brief Find the last node in a list
 *
 * @param lst Pointer to the first node in the list
 * @return Pointer to the last node in the list
 */
t_list	*ft_lstlast(t_list *lst);

/**
 * @brief Add a new node at the end of the list
 *
 * @param lst Address of a pointer to the first node in the list
 * @param new Pointer to the node to be added
 */
void	ft_lstadd_back(t_list **lst, t_list *new_node);

/**
 * @brief Delete a single node from the list
 *
 * Frees the memory of the node's content using the provided
 * function, then frees the node itself.
 *
 * @param lst Pointer to the node to be deleted
 * @param del Function pointer used to delete the node's content
 */
void	ft_lstdelone(t_list *lst, void (*del)(void *));

/**
 * @brief Delete all nodes in a list
 *
 * Deletes and frees each node in the list and sets the
 * pointer to the list to NULL.
 *
 * @param lst Address of a pointer to the first node in the list
 * @param del Function pointer used to delete each node's content
 */
void	ft_lstclear(t_list **lst, void (*del)(void *));

/**
 * @brief Apply a function to each node's content
 *
 * @param lst Pointer to the first node in the list
 * @param f Function to apply to each node's content
 */
void	ft_lstiter(t_list *lst, void (*f)(void *));

/**
 * @brief Create a new list by applying a function to each node
 *
 * Creates a new list where each node's content is the result of
 * applying the function 'f' to the corresponding node's content
 * in the original list.
 *
 * @param lst Pointer to the first node in the original list
 * @param f Function to apply to each node's content
 * @param del Function to delete content if an allocation fails
 * @return Pointer to the new list, or NULL if allocation fails
 */
t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));

#endif
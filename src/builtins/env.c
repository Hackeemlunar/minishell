/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sngantch <sngantch@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 20:57:54 by sngantch          #+#    #+#             */
/*   Updated: 2025/05/27 18:28:18 by sngantch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

void	env(char **argv, t_table *table)
{
	int		i;
	t_env	*node;

	if (argv[1])
	{
		ft_printf("env: %s: No such file or directory", argv[1]);
		write(STDERR_FILENO, "\n", 1);
		add_env(table, "?", "127");
		return ;
	}
	i = -1;
	while (++i < HASH_SIZE)
	{
		node = table->bucket[i];
		while (node)
		{
			if (node->value && node->key && (ft_strcmp(node->key, "?") != 0))
				ft_printf("%s=%s\n", node->key, node->value);
			node = node->next;
		}
	}
}

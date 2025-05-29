/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 00:00:00 by hmensah-          #+#    #+#             */
/*   Updated: 2025/01/27 00:00:00 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

char	*handle_quoted_string(char *str, t_allocs *allocs)
{
	char	*work_str;
	size_t	str_len;

	str_len = ft_strlen(str);
	if (str_len >= 2 && str[str_len - 1] == '"')
	{
		work_str = arena_alloc(allocs->exec_alloc, str_len - 1);
		if (!work_str)
			return (NULL);
		ft_strlcpy(work_str, str + 1, str_len - 1);
		return (work_str);
	}
	return (str + 1);
}

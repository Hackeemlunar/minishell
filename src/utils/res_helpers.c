/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   res_helpers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:33:12 by hmensah-          #+#    #+#             */
/*   Updated: 2025/05/16 17:27:45 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_result	create_success(void *value)
{
	t_result	result;

	result.is_error = false;
	result.data.error = NO_ERROR;
	result.data.value = value;
	return (result);
}

t_result	create_error(t_error error_code)
{
	t_result	result;

	result.is_error = true;
	result.data.error = error_code;
	return (result);
}

bool	is_special_char(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == ' '
		|| c == '(' || c == ')' || c == '&' || c == '\t'
		|| c == '\n' || c == '\0');
}

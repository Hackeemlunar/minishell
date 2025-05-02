/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmensah- <hmensah-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 18:05:45 by hmensah-          #+#    #+#             */
/*   Updated: 2025/05/02 21:35:28 by hmensah-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"


t_result	parse_cmdln(char *cmdln, t_mshell *shell, t_allocs *allocs)
{
	t_result	lex_tokens;
	t_token		*head_token;

	lex_tokens = lex_cmdln(cmdln, allocs);
	if (lex_tokens.is_error)
		return (lex_tokens);
	head_token = lex_tokens.data.value;
	printf("head token is %s\n", head_token->value);
	(void)shell;
	return lex_tokens;
}

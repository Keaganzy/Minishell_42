/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils_new.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 16:44:29 by ksng              #+#    #+#             */
/*   Updated: 2026/01/16 12:46:51 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

void	skip_spaces(t_parser *p)
{
	while (p->current && p->current->type == T_SPACE)
		p->current = p->current->next;
}

t_token	*peek(t_parser *p)
{
	return (p->current);
}

t_token	*advance(t_parser *p)
{
	t_token	*tmp;

	tmp = p->current;
	if (p->current)
		p->current = p->current->next;
	skip_spaces(p);
	return (tmp);
}

int	match(t_parser *p, t_token_type type)
{
	if (!p->current)
		return (0);
	return (p->current->type == type);
}

t_token	*expect(t_parser *p, t_token_type type)
{
	if (!match(p, type))
		return (NULL);
	return (advance(p));
}

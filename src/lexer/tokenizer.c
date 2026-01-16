/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 10:19:49 by jotong            #+#    #+#             */
/*   Updated: 2026/01/16 10:19:51 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

t_token	*token_new(t_token_type type, const char *value, int fd)
{
	t_token	*tok;

	tok = malloc(sizeof(t_token));
	if (!tok)
		return (NULL);
	tok->type = type;
	if (value)
		tok->value = ft_strdup(value);
	else
		tok->value = NULL;
	tok->fd = fd;
	tok->next = NULL;
	return (tok);
}

void	add_token_back(t_token **lst, t_token *new)
{
	t_token	*curr;

	if (!lst || !new)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	curr = *lst;
	while (curr->next)
		curr = curr->next;
	curr->next = new;
}

void	token_free(t_token *tok)
{
	if (!tok)
		return ;
	if (tok->value)
		free(tok->value);
	free(tok);
}

void	token_free_all(t_token **lst)
{
	t_token	*curr;
	t_token	*next;

	if (!lst)
		return ;
	curr = *lst;
	while (curr)
	{
		next = curr->next;
		token_free(curr);
		curr = next;
	}
	*lst = NULL;
}

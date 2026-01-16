/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd_redirect_new.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 12:48:32 by jotong            #+#    #+#             */
/*   Updated: 2026/01/16 12:51:49 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

t_ast	*parser_word(t_parser *p)
{
	t_ast	*node;
	int		word_count;
	int		i;

	word_count = count_words(p);
	if (word_count == 0)
		return (NULL);
	node = create_node(N_CMD);
	if (!node)
		return (NULL);
	node->argv = malloc(sizeof(char *) * (word_count + 1));
	if (!node->argv)
		return (free(node), NULL);
	i = 0;
	while (i < word_count)
	{
		node->argv[i] = p->current->value;
		p->current->value = NULL;
		if (!node->argv[i])
			return (free_ast(node), NULL);
		advance(p);
		i++;
	}
	node->argv[i] = NULL;
	return (node);
}

t_ast	*parse_command(t_parser *p, t_shell *shell)
{
	t_ast	*node;

	if (match(p, T_OPEN_BRACKET))
	{
		advance(p);
		node = parse_logical(p, shell);
		if (!node)
			return (NULL);
		if (!expect(p, T_CLOSE_BRACKET))
		{
			free_ast(node);
			return (NULL);
		}
		return (node);
	}
	if (match(p, T_WORD))
		return (parser_word(p));
	return (NULL);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 15:25:56 by jotong            #+#    #+#             */
/*   Updated: 2026/01/14 21:38:27 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

int	parse_quotes(char **wrd, char *s, t_token **tokens, size_t *i)
{
	char	*extracted;

	if (s[*i] != '"' && s[*i] != '\'')
		return (0);
	extracted = extract_word_with_inv_commas(s, i);
	if (*wrd)
		*wrd = ft_strjoin_and_free(wrd, extracted, 3);
	else
		*wrd = extracted;
	if (!(s[*i] == '\0' || s[*i] == ' '))
		return (1);
	add_token_back(tokens, token_new(T_WORD, *wrd, -1));
	free(*wrd);
	*wrd = NULL;
	return (1);
}

int	parse_word(char **word, char *s, t_token **tokens, size_t *i)
{
	if (s[*i] == '\0')
		return (0);
	*word = ft_strjoin_and_free(word, extract_word(s, i), 3);
	if (s[*i] == ' ' || s[*i] == '\0' || s[*i] == '<' || s[*i] == '>'
		|| s[*i] == '(' || s[*i] == ')' || s[*i] == '&' || s[*i] == '|')
	{
		add_token_back(tokens, token_new(T_WORD, *word, -1));
		free(*word);
		*word = NULL;
	}
	return (1);
}

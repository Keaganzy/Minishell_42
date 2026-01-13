/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_checks3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 15:41:16 by jotong            #+#    #+#             */
/*   Updated: 2026/01/02 15:41:40 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

static int	is_triple_ampersand(char *s, int i)
{
	if (s[i + 2] == '&')
		return (1);
	return (0);
}

static int	is_double_ampersand(char *s, int i)
{
	if (s[i + 1] == '&')
		return (1);
	return (0);
}

static int	check_ampersand_at_pos(char *s, int i)
{
	if (!s[i + 1])
		return (0);
	if (is_double_ampersand(s, i))
	{
		if (is_triple_ampersand(s, i))
			return (0);
		return (2);
	}
	return (0);
}

static int	process_ampersand(char *s, int *i, int is_in_quotes)
{
	int	result;

	if (!is_in_quotes && s[*i] == '&')
	{
		result = check_ampersand_at_pos(s, *i);
		if (result == 0)
			return (0);
		if (result == 2)
			*i += 1;
	}
	return (1);
}

int	check_valid_ampersand(char *s)
{
	int	i;
	int	is_in_quotes;

	i = 0;
	is_in_quotes = 0;
	while (s[i])
	{
		if (s[i] == '\'' || s[i] == '"')
			is_in_quotes = !is_in_quotes;
		if (!process_ampersand(s, &i, is_in_quotes))
			return (0);
		i++;
	}
	return (1);
}

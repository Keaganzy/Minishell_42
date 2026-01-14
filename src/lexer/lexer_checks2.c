/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_checks2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 20:01:59 by jotong            #+#    #+#             */
/*   Updated: 2026/01/14 20:02:01 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

static void	handle_inv_comma_count(char c, int *inv_comma, int *d_inv_comma)
{
	if (c == '"')
	{
		if (*inv_comma == 1)
			return ;
		else if (*d_inv_comma == 1)
			(*d_inv_comma)--;
		else
			(*d_inv_comma)++;
	}
	else if (c == '\'')
	{
		if (*d_inv_comma == 1)
			return ;
		else if (*inv_comma == 1)
			(*inv_comma)--;
		else
			(*inv_comma)++;
	}
	return ;
}

int	check_commas_closed(char *s)
{
	int	i;
	int	d_inv_comma;
	int	inv_comma;

	i = 0;
	d_inv_comma = 0;
	inv_comma = 0;
	while (s[i])
	{
		if (s[i] == '"' || s[i] == '\'')
			handle_inv_comma_count(s[i], &inv_comma, &d_inv_comma);
		i++;
	}
	if (d_inv_comma != 0 || inv_comma != 0)
		return (0);
	return (1);
}

int	check_invalid_pipes(char *s)
{
	int	i;
	int	s_back;

	i = 0;
	s_back = ft_strlen(s);
	while (s[i] && s[i] == ' ')
		i++;
	while (s[s_back] && s[s_back] == ' ')
		s_back--;
	if (s[i] == '|' || s[i] == '<' || s[i] == '>')
		return (0);
	if (s[s_back] == '|' || s[s_back] == '<' || s[s_back] == '>')
		return (0);
	return (1);
}

// int	check_valid_ampersand(char *s)
// {
// 	int	i;
// 	int	is_in_quotes;

// 	i = 0;
// 	is_in_quotes = 0;
// 	while (s[i])
// 	{
// 		if (s[i] == '\'' || s[i] == '"')
// 			is_in_quotes = !is_in_quotes;
// 		if (!is_in_quotes)
// 		{
// 			if (s[i] == '&')
// 			{
// 				if (!s[i + 1])
// 					return (0);
// 				else if (s[i + 1] == '&')
// 				{
// 					if (s[i + 2] == '&')
// 						return (0);
// 					i += 1;
// 				}
// 				else
// 					return (0);
// 			}
// 		}
// 		i++;
// 	}
// 	return (1);
// }

int	check_valid_brackets(char *s)
{
	int	i;
	int	is_in_quotes;

	i = 0;
	is_in_quotes = 0;
	while (s[i])
	{
		if (s[i] == '\'' || s[i] == '"')
			is_in_quotes = !is_in_quotes;
		if (!is_in_quotes)
		{
			if (s[i] == '{' || s[i] == '}')
				return (0);
		}
		i++;
	}
	return (1);
}

int	check_has_semicolon(char *s)
{
	int	i;
	int	is_in_quotes;

	i = 0;
	is_in_quotes = 0;
	while (s[i])
	{
		if (s[i] == '\'' || s[i] == '"')
			is_in_quotes = !is_in_quotes;
		if (!is_in_quotes)
		{
			if (s[i] == ';')
				return (0);
		}
		i++;
	}
	return (1);
}

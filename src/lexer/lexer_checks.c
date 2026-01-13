/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_checks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 15:36:30 by jotong            #+#    #+#             */
/*   Updated: 2026/01/11 09:55:28 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

static int	check_valid_redirs(char *s)
{
	int		s_len;
	size_t	i;

	s_len = ft_strlen(s);
	i = s_len - 1;
	while (i > 0)
	{
		if (s[i] == ' ')
			i--;
		else if (s[i] == '>' || s[i] == '<')
			return (0);
		else
			return (1);
		i--;
	}
	return (1);
}

int	check_syntax(char *s)
{
	if (!s || !*s || (ft_strlen(s) == 1 && *s == ' '))
		return (0);
	if (check_has_equals(s) == 0)
		return (0);
	if (check_commas_closed(s) == 0)
		return (printf("Unclosed inv commas.\n"), 0);
	if (check_valid_ampersand(s) == 0)
		return (printf("Syntax error near unexpected token '&'\n"), 0);
	if (check_valid_redirs(s) == 0)
		return (printf("Syntax error near unexpected token 'newline'\n"), 0);
	if (check_valid_ampersand(s) == 0)
		return (printf("Syntax error near unexpected token '&'\n"), 0);
	if (check_valid_brackets(s) == 0)
		return (printf("Curly brackets not supported in this MS.\n"), 0);
	if (check_has_semicolon(s) == 0)
		return (printf("Semicolons not supported in this MS.\n"), 0);
	return (1);
}

int	check_has_equals(char *s)
{
	char	**substr;

	if (s[0] == '"' || s[0] == '\'')
		substr = ft_split(s, s[0]);
	else
		substr = ft_split(s, ' ');
	if (!substr || !(substr[0]))
		return (0);
	if (ft_strnstr(substr[0], "=", ft_strlen(substr[0])) != NULL)
	{
		printf("command \'%s\' not found.\n", substr[0]);
		free_array(substr);
		return (0);
	}
	free_array(substr);
	return (1);
}

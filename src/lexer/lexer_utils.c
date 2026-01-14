/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 17:16:53 by jotong            #+#    #+#             */
/*   Updated: 2026/01/14 21:31:52 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

char	*ft_strndup(const char *src, size_t num)
{
	size_t		len;
	char		*dup;

	len = ft_strlen(src);
	if (len > num)
		len = num;
	dup = (char *)malloc(len + 1);
	if (!dup)
		return (NULL);
	else
	{
		ft_memcpy(dup, src, len);
		dup[len] = '\0';
		return (dup);
	}
}

int	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n'
		|| c == '\v' || c == '\f' || c == '\r');
}

char	*extract_word(const char *s, size_t *i)
{
	size_t	start;

	start = *i;
	while (s[*i] && !is_space((unsigned char)s[*i]) && s[*i] != '|'
		&& s[*i] != '<' && s[*i] != '>' && s[*i] != '(' && s[*i] != ')'
		&& s[*i] != '"' && s[*i] != '\'' && s[*i] != '&')
	{
		(*i)++;
	}
	return (ft_strndup(s + start, *i - start));
}

char	*extract_till_next_inv_comma(const char *s, size_t *i)
{
	size_t			start;
	unsigned char	inv_comma;

	inv_comma = s[*i];
	(*i)++;
	start = *i;
	while (s[*i] && s[*i] != inv_comma)
		(*i)++;
	(*i)++;
	return (ft_strndup(s + start, *i - start - 1));
}

t_token_type	get_op_type(const char *s, size_t *i)
{
	if (s[*i] == '<')
	{
		if (s[*i + 1] == '<')
			return (*i += 2, T_HEREDOC);
		return ((*i)++, T_REDIR_IN);
	}
	else if (s[*i] == '>')
	{
		if (s[*i + 1] == '>')
			return (*i += 2, T_REDIR_APPEND);
		return ((*i)++, T_REDIR_OUT);
	}
	else if (s[*i] == '(')
		return ((*i)++, T_OPEN_BRACKET);
	else if (s[*i] == ')')
		return ((*i)++, T_CLOSE_BRACKET);
	else if (s[*i] == '&' && s[*i + 1] == '&')
		return ((*i) += 2, T_AND);
	else if (s[*i] == '|' && s[*i + 1] == '|')
		return ((*i) += 2, T_OR);
	else if (s[*i] == '|')
		return ((*i)++, T_PIPE);
	else if (is_space((unsigned char)s[*i]))
		return (handle_space(s, i));
	return (T_WORD);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_reporting_new.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksng <ksng@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 15:36:27 by ksng              #+#    #+#             */
/*   Updated: 2025/11/18 19:03:31 by ksng             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                           ERROR REPORTING                                  */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

//remove if not used

static int	count_position(t_parser *p)
{
	int		pos;
	t_token	*tmp;

	pos = 0;
	tmp = p->tokens;
	while (tmp && tmp != p->current)
	{
		pos++;
		tmp = tmp->next;
	}
	return (pos);
}

static const char	*token_type_str(t_token_type type)
{
	if (type == T_WORD)
		return ("word");
	if (type == T_PIPE)
		return ("'|'");
	if (type == T_REDIR_IN)
		return ("'<'");
	if (type == T_REDIR_OUT)
		return ("'>'");
	if (type == T_REDIR_APPEND)
		return ("'>>'");
	if (type == T_HEREDOC)
		return ("'<<'");
	if (type == T_OPEN_BRACKET)
		return ("'('");
	if (type == T_CLOSE_BRACKET)
		return ("')'");
	if (type == T_AND)
		return ("'&&'");
	if (type == T_OR)
		return ("'||'");
	return ("unknown token");
}

static void	print_error_message(int pos, const char *expected,
		const char *got)
{
	ft_putstr_fd("minishell: syntax error at token ", 2);
	ft_putnbr_fd(pos, 2);
	ft_putstr_fd(": expected ", 2);
	ft_putstr_fd(expected, 2);
	ft_putstr_fd(", got ", 2);
	ft_putstr_fd(got, 2);
	ft_putstr_fd("\n", 2);
}

void	syntax_error(t_parser *p, const char *expected)
{
	int			pos;
	const char	*got;

	pos = count_position(p);
	if (p->current)
		got = token_type_str(p->current->type);
	else
		got = "EOF";
	print_error_message(pos, expected, got);
}

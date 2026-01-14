/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 20:01:41 by jotong            #+#    #+#             */
/*   Updated: 2026/01/14 21:36:58 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

static int	handle_whitespace(const char *s, size_t *i, t_token **tokens)
{
	if (is_space((unsigned char)s[*i]))
	{
		add_token_back(tokens, token_new(get_op_type(s, i), NULL, -1));
		return (1);
	}
	return (0);
}

static int	handle_fd_redir(const char *s, size_t *i, t_token **tokens,
		char **word)
{
	int				fd;
	t_token_type	type;

	if (is_fd_redir(s, *i))
	{
		fd = extract_fd_for_lexer(s, i);
		type = get_op_type(s, i);
		add_token_back(tokens, token_new(type, NULL, fd));
		*word = NULL;
		return (1);
	}
	return (0);
}

static int	handle_operator(const char *s, size_t *i, t_token **tokens,
		char **word)
{
	if (s[*i] == '|' || s[*i] == '<' || s[*i] == '>' || s[*i] == '('
		|| s[*i] == ')' || s[*i] == '&')
	{
		add_token_back(tokens, token_new(get_op_type(s, i), NULL, -1));
		*word = NULL;
		return (1);
	}
	return (0);
}

static int	process_char(const char *s, size_t *i, t_token **tokens,
		char **word)
{
	if (handle_whitespace(s, i, tokens))
		return (1);
	if (parse_quotes(word, (char *)s, tokens, i))
		return (1);
	if (handle_fd_redir(s, i, tokens, word))
		return (1);
	if (handle_operator(s, i, tokens, word))
		return (1);
	if (parse_word(word, (char *)s, tokens, i))
		return (1);
	return (0);
}

t_token	*lex_input(const char *s, t_shell *shell)
{
	size_t	i;
	t_token	*tokens;
	char	*word;

	i = 0;
	tokens = NULL;
	word = NULL;
	if (check_syntax((char *)s) == 0)
	{
		shell->last_exit_status = 2;
		return (NULL);
	}
	while (s[i])
		process_char(s, &i, &tokens, &word);
	return (tokens);
}

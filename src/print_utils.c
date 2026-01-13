/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 14:53:29 by ksng              #+#    #+#             */
/*   Updated: 2026/01/02 15:05:58 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

void	print_token_stream_detailed(t_token *tokens)
{
	t_token	*curr;
	int		i;

	curr = tokens;
	i = 0;
	printf("\nDetailed Token Stream:\n");
	while (curr)
	{
		printf("[%d:%s", i, token_type_to_string(curr->type));
		if (curr->value && curr->value[0])
			printf(":\"%s\"", curr->value);
		printf("]");
		if (curr->next)
			printf(" → ");
		curr = curr->next;
		i++;
		if (i > 0 && i % 5 == 0 && curr)
			printf("\n    ");
		if (i > 100)
		{
			printf("\n[ERROR: Too many tokens]\n");
			break ;
		}
	}
	printf("\n\n");
}

static const char	*get_token_color(t_token_type type)
{
	if (type == T_WORD)
		return ("\033[0;36m");
	if (type == T_PIPE)
		return ("\033[0;35m");
	if (type == T_REDIR_IN || type == T_REDIR_OUT
		|| type == T_REDIR_APPEND || type == T_HEREDOC)
		return ("\033[0;33m");
	if (type == T_OPEN_BRACKET || type == T_CLOSE_BRACKET)
		return ("\033[0;32m");
	if (type == T_AND || type == T_OR)
		return ("\033[0;31m");
	return ("\033[0m");
}

void	print_token_stream_colored(t_token *tokens)
{
	t_token		*curr;
	int			count;
	const char	*color;

	curr = tokens;
	count = 0;
	printf("\nToken Stream (Colored):\n");
	while (curr)
	{
		color = get_token_color(curr->type);
		printf("%s[", color);
		if (curr->value && curr->value[0])
			printf("%s]\033[0m", curr->value);
		else
			printf("%s]\033[0m", token_type_to_string(curr->type));
		if (curr->next)
			printf(" → ");
		curr = curr->next;
		if (++count > 100)
		{
			printf("\033[0;31m [ERROR: Too many tokens]\033[0m");
			break ;
		}
	}
	printf("\n\n");
}

void	print_token_stream_compact(t_token *tokens)
{
	t_token	*curr;

	curr = tokens;
	while (curr)
	{
		if (curr->value && curr->value[0])
			printf("%s", curr->value);
		else
			printf("<%s>", token_type_to_string(curr->type));
		if (curr->next)
			printf(" ");
		curr = curr->next;
	}
	printf("\n");
}

void	print_token_diagnostic(t_token *tokens)
{
	t_token	*curr;
	int		stats[4];

	curr = tokens;
	ft_memset(stats, 0, sizeof(int) * 4);
	printf("\n=== TOKEN DIAGNOSTIC ===\nStream: ");
	while (curr && stats[0] < 100)
	{
		if (curr->value && curr->value[0])
			printf("[%s]", curr->value);
		else
			printf("[%s]", token_type_to_string(curr->type));
		if (curr->next)
			printf(" → ");
		if (curr->type == T_WORD)
			stats[1]++;
		else if (curr->type == T_SPACE)
			stats[2]++;
		else
			stats[3]++;
		curr = curr->next;
		stats[0]++;
	}
	printf("\nTotal tokens: %d\n  Words: %d\n", stats[0], stats[1]);
	printf("  Spaces: %d\n  Operators: %d\n", stats[2], stats[3]);
}

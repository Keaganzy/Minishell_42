/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 15:01:12 by jotong            #+#    #+#             */
/*   Updated: 2026/01/02 15:05:32 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

void	print_node_detailed(t_ast *node, int depth, char *position)
{
	int	i;

	i = 0;
	while (i++ < depth)
		printf("  ");
	printf("┌─ %s ─────────────\n", position);
	print_node_detailed_body(node, depth);
	if (node->argv)
	{
		i = 0;
		while (i++ < depth)
			printf("  ");
		printf("│ Arguments:\n");
		print_args_detailed(node->argv, depth);
	}
	i = 0;
	while (i++ < depth)
		printf("  ");
	printf("└────────────────────\n");
}

void	print_ast_detailed(t_ast *node, int depth, char *position)
{
	if (!node)
		return ;
	print_node_detailed(node, depth, position);
	if (node->left)
		print_ast_detailed(node->left, depth + 1, "LEFT");
	if (node->right)
		print_ast_detailed(node->right, depth + 1, "RIGHT");
}

void	print_ast(t_ast *root)
{
	if (!root)
	{
		printf("AST is NULL\n");
		return ;
	}
	printf("\n");
	printf("=====================================\n");
	printf("         AST VISUALIZATION           \n");
	printf("=====================================\n");
	print_ast_tree(root);
	printf("=====================================\n");
	printf("       DETAILED NODE INFO            \n");
	printf("=====================================\n");
	print_ast_detailed(root, 0, "ROOT");
	printf("=====================================\n");
	printf("\n");
}

const char	*token_type_to_string(t_token_type type)
{
	if (type == T_WORD)
		return ("WORD");
	if (type == T_PIPE)
		return ("PIPE");
	if (type == T_REDIR_IN)
		return ("REDIR_IN");
	if (type == T_REDIR_OUT)
		return ("REDIR_OUT");
	if (type == T_REDIR_APPEND)
		return ("REDIR_APPEND");
	if (type == T_HEREDOC)
		return ("HEREDOC");
	if (type == T_OPEN_BRACKET)
		return ("OPEN_BRACKET");
	if (type == T_CLOSE_BRACKET)
		return ("CLOSE_BRACKET");
	if (type == T_AND)
		return ("AND");
	if (type == T_OR)
		return ("OR");
	if (type == T_SPACE)
		return ("SPACE");
	return ("UNKNOWN");
}

void	print_tokens(t_token *token)
{
	int	i;

	i = 0;
	printf("\nToken Stream: ");
	while (token)
	{
		printf("[[%d] %s", i, token_type_to_string(token->type));
		if (token->value)
			printf(": {%s}", token->value);
		printf("]");
		if (token->next)
			printf(" ");
		token = token->next;
		i++;
		if (i > 100)
		{
			printf(" [ERROR: Too many tokens]");
			break ;
		}
	}
	printf("\n\n");
}

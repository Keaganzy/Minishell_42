/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 15:01:17 by jotong            #+#    #+#             */
/*   Updated: 2026/01/02 15:01:18 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

const char	*node_type_to_string(t_node_type type)
{
	if (type == N_CMD)
		return ("CMD");
	if (type == N_PIPE)
		return ("PIPE");
	if (type == N_REDIR_IN)
		return ("REDIR_IN");
	if (type == N_REDIR_OUT)
		return ("REDIR_OUT");
	if (type == N_REDIR_APPEND)
		return ("REDIR_APPEND");
	if (type == N_HEREDOC)
		return ("HEREDOC");
	if (type == N_AND)
		return ("AND");
	if (type == N_OR)
		return ("OR");
	return ("UNKNOWN");
}

void	print_node_argv(t_ast *node, int depth)
{
	int	i;

	i = 0;
	while (i < depth)
	{
		printf("  ");
		i++;
	}
	printf("  args: [");
	i = 0;
	while (node->argv[i])
	{
		printf("\"%s\"", node->argv[i]);
		if (node->argv[i + 1])
			printf(", ");
		i++;
	}
	printf("]\n");
}

void	print_node_info(t_ast *node, int depth)
{
	int	i;

	i = 0;
	while (i < depth)
	{
		printf("  ");
		i++;
	}
	printf("Node: %s\n", node_type_to_string(node->type));
	if (node->filename)
	{
		i = 0;
		while (i < depth)
		{
			printf("  ");
			i++;
		}
		printf("  filename: \"%s\"\n", node->filename);
	}
	if (node->argv)
		print_node_argv(node, depth);
}

void	print_ast_simple(t_ast *node, int depth)
{
	if (!node)
		return ;
	print_node_info(node, depth);
	if (node->left)
		print_ast_simple(node->left, depth + 1);
	if (node->right)
		print_ast_simple(node->right, depth + 1);
}

void	print_indent(int depth, int is_last[], int is_right)
{
	int	i;

	i = 0;
	while (i < depth - 1)
	{
		if (is_last[i])
			printf("    ");
		else
			printf("│   ");
		i++;
	}
	if (depth > 0)
	{
		if (is_right)
			printf("└── ");
		else
			printf("├── ");
	}
}

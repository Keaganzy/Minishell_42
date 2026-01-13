/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 14:53:36 by jotong         #+#    #+#             */
/*   Updated: 2026/01/02 15:01:00 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

void	print_node_compact(t_ast *node)
{
	int	i;

	printf("%s", node_type_to_string(node->type));
	if (node->filename)
		printf(" (\"%s\")", node->filename);
	if (node->argv)
	{
		printf(" [");
		i = 0;
		while (node->argv[i])
		{
			printf("%s", node->argv[i]);
			if (node->argv[i + 1])
				printf(" ");
			i++;
		}
		printf("]");
	}
	printf("\n");
}

void	print_tree_recursive(t_ast *node, int depth,
		int is_last[], int is_right)
{
	if (!node)
		return ;
	print_indent(depth, is_last, is_right);
	print_node_compact(node);
	if (node->left || node->right)
	{
		if (depth < 50)
			is_last[depth] = 0;
		if (node->left)
			print_tree_recursive(node->left, depth + 1, is_last, 0);
		if (depth < 50)
			is_last[depth] = 1;
		if (node->right)
			print_tree_recursive(node->right, depth + 1, is_last, 1);
	}
}

void	print_ast_tree(t_ast *root)
{
	int	is_last[50];
	int	i;

	if (!root)
	{
		printf("(empty tree)\n");
		return ;
	}
	i = 0;
	while (i < 50)
	{
		is_last[i] = 0;
		i++;
	}
	printf("\nAST Tree:\n");
	print_tree_recursive(root, 0, is_last, 0);
	printf("\n");
}

void	print_args_detailed(char **args, int depth)
{
	int	i;
	int	j;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		j = 0;
		while (j < depth)
		{
			printf("  ");
			j++;
		}
		printf("    [%d]: %s\n", i, args[i]);
		i++;
	}
}

void	print_node_detailed_body(t_ast *node, int depth)
{
	int	i;

	i = 0;
	while (i++ < depth)
		printf("  ");
	printf("│ Type: %s\n", node_type_to_string(node->type));
	if (node->filename)
	{
		i = 0;
		while (i++ < depth)
			printf("  ");
		printf("│ Filename: \"%s\"\n", node->filename);
		if (node->heredoc_content)
		{
			i = 0;
			while (i++ < depth)
				printf("  ");
			printf("│ Here_Content: \"%s\"\n", node->heredoc_content);
		}
	}
}

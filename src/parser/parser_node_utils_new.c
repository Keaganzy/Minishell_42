/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_node_utils_new.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 12:24:56 by jotong            #+#    #+#             */
/*   Updated: 2026/01/16 12:25:13 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

t_ast	*create_node(t_node_type type)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = type;
	node->argv = NULL;
	node->filename = NULL;
	node->heredoc_content = NULL;
	node->left = NULL;
	node->right = NULL;
	node->fd = -1;
	return (node);
}

t_ast	*create_binary_node(t_node_type type, t_ast *left, t_ast *right)
{
	t_ast	*node;

	node = create_node(type);
	if (!node)
		return (NULL);
	node->left = left;
	node->right = right;
	return (node);
}

t_ast	*create_redir_node(t_node_type type, char *filename,
				t_ast *cmd, int fd)
{
	t_ast	*node;

	node = create_node(type);
	if (!node)
		return (NULL);
	node->filename = ft_strdup(filename);
	if (!node->filename)
	{
		free(node);
		return (NULL);
	}
	node->left = cmd;
	if (fd == -1)
	{
		if (type == N_REDIR_IN || type == N_HEREDOC)
			node->fd = 0;
		else
			node->fd = 1;
	}
	else
		node->fd = fd;
	return (node);
}

t_ast	*new_ast(t_node_type type)
{
	t_ast	*ast;

	ast = ft_calloc(1, sizeof(t_ast));
	ast->type = type;
	ast->argv = NULL;
	return (ast);
}

void	free_ast(t_ast *node)
{
	int	i;

	if (!node)
		return ;
	free_ast(node->left);
	free_ast(node->right);
	if (node->argv)
	{
		i = 0;
		while (node->argv[i])
		{
			free(node->argv[i]);
			i++;
		}
		free(node->argv);
	}
	if (node->filename)
		free(node->filename);
	if (node->heredoc_content)
		free(node->heredoc_content);
	free (node);
}

// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   parser_utils.c                                     :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: ksng <ksng@student.42.fr>                  +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2025/11/12 13:14:20 by ksng              #+#    #+#             */
// /*   Updated: 2025/11/12 17:09:41 by ksng             ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include "minishell.h"
// #include "libft.h"

// t_ast	*new_ast(t_node_type type)
// {
// 	t_ast *ast;

// 	ast = ft_calloc(1, sizeof(t_ast));
// 	ast->type = type;
// 	ast->argv = NULL;
// 	return (ast);
// }

// void free_ast(t_ast *node)
// {
//     int i;

//     if (!node)
//         return;

//     // First, recursively free children (DO THIS FIRST!)
//     free_ast(node->left);
//     free_ast(node->right);

//     // Then free the node's data
//     if (node->argv)
//     {
//         i = 0;
//         while (node->argv[i])
//         {
//             free(node->argv[i]);
//             i++;
//         }
//         free(node->argv);
//     }

//     if (node->filename)
//         free(node->filename);

//     // Finally, free the node itself (DO THIS LAST!)
//     free(node);

//     // DON'T access node after this point!
// }


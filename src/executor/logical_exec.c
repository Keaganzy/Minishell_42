/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logical_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 21:58:39 by ksng              #+#    #+#             */
/*   Updated: 2026/01/16 10:26:22 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

int execute_logical(t_ast *node, t_shell *shell)
{
    int left_status;
    int right_status;
    left_status = execute_node(node->left, shell);
    shell->last_exit_status = left_status;
    if (g_sigint_received)
    {
        g_sigint_received = 0;
        return (130);
    }
    if (node->type == N_AND)
    {
        if (left_status == 0)
        {
            right_status = execute_node(node->right, shell);
            shell->last_exit_status = right_status;
            if (g_sigint_received)
            {
                g_sigint_received = 0;
                return (130);
            }
            return (right_status);
        }
        return (left_status);
    }
    if (node->type == N_OR)
    {
        if (left_status != 0)
        {
            right_status = execute_node(node->right, shell);
            shell->last_exit_status = right_status;
            if (g_sigint_received)
            {
                g_sigint_received = 0;
                return (130);
            }
            return (right_status);
        }
        return (left_status);
    }
    return (1);
}

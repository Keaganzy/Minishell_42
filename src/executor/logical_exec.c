/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logical_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 21:58:39 by ksng              #+#    #+#             */
/*   Updated: 2026/01/11 14:14:25 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

// int	execute_logical(t_ast *node, t_shell *shell)
// {
// 	int	left_status;
// 	int	right_status;

// 	left_status = execute_node(node->left, shell);
// 	shell->last_exit_status = left_status;
// 	if (node->type == N_AND)
// 	{
// 		if (left_status == 0)
// 		{
// 			right_status = execute_node(node->right, shell);
// 			shell->last_exit_status = right_status;
// 			return (right_status);
// 		}
// 		return (left_status);
// 	}
// 	if (node->type == N_OR)
// 	{
// 		if (left_status != 0)
// 		{
// 			right_status = execute_node(node->right, shell);
// 			shell->last_exit_status = right_status;
// 			return (right_status);
// 		}
// 		return (left_status);
// 	}
// 	return (1);
// }

// int	execute_logical(t_ast *node, t_shell *shell)
// {
// 	int	left_status;
// 	int	right_status;

// 	left_status = execute_node(node->left, shell);
// 	shell->last_exit_status = left_status;
	
// 	// If interrupted by signal (status 130 for SIGINT), stop execution
// 	if (left_status > 128)
// 		return (left_status);
	
// 	if (node->type == N_AND)
// 	{
// 		if (left_status == 0)
// 		{
// 			right_status = execute_node(node->right, shell);
// 			shell->last_exit_status = right_status;
// 			return (right_status);
// 		}
// 		return (left_status);
// 	}
// 	if (node->type == N_OR)
// 	{
// 		if (left_status != 0)
// 		{
// 			right_status = execute_node(node->right, shell);
// 			shell->last_exit_status = right_status;
// 			return (right_status);
// 		}
// 		return (left_status);
// 	}
// 	return (1);
// }
int execute_logical(t_ast *node, t_shell *shell)
{
    int left_status;
    int right_status;
    /* Execute left side */
    left_status = execute_node(node->left, shell);
    shell->last_exit_status = left_status;
    /* Check if we were interrupted by a signal */
    if (g_sigint_received)
    {
        /* Don't execute right side if interrupted */
        g_sigint_received = 0;
        return (130);  /* Return SIGINT exit code */
    }
    if (node->type == N_AND)
    {
        /* Only execute right side if left succeeded (exit code 0) */
        if (left_status == 0)
        {
            right_status = execute_node(node->right, shell);
            shell->last_exit_status = right_status;
            /* Check if we were interrupted by a signal */
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
        /* Execute right side if left failed (exit code != 0) */
        if (left_status != 0)
        {
            right_status = execute_node(node->right, shell);
            shell->last_exit_status = right_status;
            /* Check if we were interrupted by a signal */
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
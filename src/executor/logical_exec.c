/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logical_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 11:18:00 by jotong            #+#    #+#             */
/*   Updated: 2026/01/16 11:18:47 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

static int	handle_or_nodes(t_ast *node, t_shell *shell, int *left_status,
	int *right_status)
{
	if (*left_status != 0)
	{
		*right_status = execute_node(node->right, shell);
		shell->last_exit_status = *right_status;
		if (g_sigint_received)
		{
			g_sigint_received = 0;
			return (130);
		}
		return (*right_status);
	}
	return (*left_status);
}

static int	handle_and_nodes(t_ast *node, t_shell *shell, int *left_status,
	int *right_status)
{
	if (*left_status == 0)
	{
		*right_status = execute_node(node->right, shell);
		shell->last_exit_status = *right_status;
		if (g_sigint_received)
		{
			g_sigint_received = 0;
			return (130);
		}
		return (*right_status);
	}
	return (*left_status);
}

int	execute_logical(t_ast *node, t_shell *shell)
{
	int	left_status;
	int	right_status;

	left_status = execute_node(node->left, shell);
	shell->last_exit_status = left_status;
	if (g_sigint_received)
	{
		g_sigint_received = 0;
		return (130);
	}
	if (node->type == N_AND)
		return (handle_and_nodes(node, shell, &left_status, &right_status));
	if (node->type == N_OR)
		return (handle_or_nodes(node, shell, &left_status, &right_status));
	return (1);
}

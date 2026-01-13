/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 19:03:59 by ksng              #+#    #+#             */
/*   Updated: 2026/01/11 14:08:52 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

int	execute_node(t_ast *node, t_shell *shell)
{
	if (!node)
		return (0);
	if (node->type == N_CMD)
		return (execute_cmd(node, shell));
	if (node->type == N_PIPE)
		return (execute_pipe(node, shell));
	if (node->type == N_REDIR_IN || node->type == N_REDIR_OUT
		|| node->type == N_REDIR_APPEND || node->type == N_HEREDOC)
		return (execute_redir(node, shell));
	if (node->type == N_AND || node->type == N_OR)
		return (execute_logical(node, shell));
	return (1);
}

int execute_ast(t_ast *root, t_shell *shell)
{
	int status;

	if (!root)
		return (0);
	status = execute_node(root, shell);
	shell->last_exit_status = status;
	return (status);
}

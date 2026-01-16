/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 20:56:01 by ksng              #+#    #+#             */
/*   Updated: 2026/01/16 11:21:22 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

static int	setup_rechain(t_ast *node, t_shell *shell)
{
	int	status;

	(void)shell;
	if (!node)
		return (0);
	if (node->left && (node->left->type == N_REDIR_IN
			|| node->left->type == N_REDIR_OUT
			|| node->left->type == N_REDIR_APPEND
			|| node->left->type == N_HEREDOC))
	{
		status = setup_rechain(node->left, shell);
		if (status != 0)
			return (1);
	}
	return (setup_redirections(node, shell));
}

static void	backup_fds(t_shell *shell)
{
	shell->stdin_backup = dup(STDIN_FILENO);
	shell->stdout_backup = dup(STDOUT_FILENO);
}

static void	restore_fds(t_shell *shell)
{
	if (shell->stdin_backup != -1)
	{
		dup2(shell->stdin_backup, STDIN_FILENO);
		close(shell->stdin_backup);
		shell->stdin_backup = -1;
	}
	if (shell->stdout_backup != -1)
	{
		dup2(shell->stdout_backup, STDOUT_FILENO);
		close(shell->stdout_backup);
		shell->stdout_backup = -1;
	}
	// TODO: STANDARD ERROR
	// if (shell->stdout_backup != -1)
	// {
	// 	dup2(shell->stdout_backup, STDOUT_FILENO);
	// 	close(shell->stdout_backup);
	// 	shell->stdout_backup = -1;
	// }
}

int	setup_redirections(t_ast *node, t_shell *shell)
{
	(void)shell;
	if (node->type == N_REDIR_IN)
		return (setup_redir_in(node, shell));
	if (node->type == N_REDIR_OUT)
		return (setup_redir_out(node, shell));
	if (node->type == N_REDIR_APPEND)
		return (setup_redir_append(node, shell));
	if (node->type == N_HEREDOC)
		return (setup_heredoc(node, shell));
	return (1);
}

int	execute_redir(t_ast *node, t_shell *shell)
{
	int		status;
	t_ast	*cmd;

	backup_fds(shell);
	status = setup_rechain(node, shell);
	if (status != 0)
	{
		restore_fds(shell);
		return (status);
	}
	cmd = node;
	while (cmd && (cmd->type == N_REDIR_IN
			|| cmd->type == N_REDIR_OUT || cmd->type == N_REDIR_APPEND
			|| cmd->type == N_HEREDOC))
	{
		cmd = cmd->left;
	}
	status = execute_node(cmd, shell);
	shell->last_exit_status = status;
	restore_fds(shell);
	return (status);
}

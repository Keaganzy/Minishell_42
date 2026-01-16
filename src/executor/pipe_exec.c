/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 20:39:15 by ksng              #+#    #+#             */
/*   Updated: 2026/01/16 10:26:38 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

static int	wait_for_children(pid_t pid1, pid_t pid2)
{
	int	status1;
	int	status2;
	int final_status;

	waitpid(pid1, &status1, 0);
	waitpid(pid2, &status2, 0);
	if (WIFEXITED(status2))
		final_status = WEXITSTATUS(status2);
	else if (WIFSIGNALED(status2))
		final_status = 128 + WTERMSIG(status2);
	else if (WIFSIGNALED(status1))
		final_status = 128 + WTERMSIG(status1);
	else
		final_status = 1;
	return (final_status);
}

static int	execute_pipe_child(t_ast *node, int *pipefd, t_shell *shell,
	int is_left)
{
	int	status;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);

	if (is_left)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		status = execute_node(node->left, shell);
	}
	else
	{
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		status = execute_node(node->right, shell);
	}
	cleanup_shell(shell);
	exit(status);
}

int	execute_pipe(t_ast *node, t_shell *shell)
{
	int		pipefd[2];
	pid_t	pid1;
	pid_t	pid2;
	int		final_status;

	if (pipe(pipefd) == -1)
		return (1);
	pid1 = fork();
	if (pid1 == -1)
		return (close(pipefd[0]), close(pipefd[1]), 1);
	if (pid1 == 0)
		execute_pipe_child(node, pipefd, shell, 1);
	pid2 = fork();
	if (pid2 == -1)
		return (close(pipefd[0]), close(pipefd[1]), 1);
	if (pid2 == 0)
		execute_pipe_child(node, pipefd, shell, 0);
	close(pipefd[0]);
	close(pipefd[1]);
	final_status = wait_for_children(pid1, pid2);
	shell->last_exit_status = final_status;
	return (final_status);
}

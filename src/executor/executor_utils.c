/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 23:15:23 by jotong            #+#    #+#             */
/*   Updated: 2026/01/16 13:07:43 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

int	is_file_descriptor(const char *str)
{
	if (!str || !*str)
		return (0);
	if (*str == '-' || *str == '+')
		return (0);
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}

int	setup_redir_append(t_ast *node, t_shell *shell)
{
	int	fd;
	int	target_fd;

	node->filename = expand_and_replace(&node->filename, shell);
	if (node->filename && is_file_descriptor(node->filename) && node->fd != 1)
	{
		target_fd = ft_atoi(node->filename);
		if (dup2(target_fd, node->fd) == -1)
			return (1);
		return (0);
	}
	fd = open(node->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		perror(node->filename);
		return (1);
	}
	dup2(fd, node->fd);
	close(fd);
	return (0);
}

int	setup_heredoc(t_ast *node, t_shell *shell)
{
	int		pipefd[2];

	(void)shell;
	if (!node->heredoc_content)
		return (1);
	if (pipe(pipefd) == -1)
		return (1);
	write(pipefd[1], node->heredoc_content, ft_strlen(node->heredoc_content));
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	return (0);
}

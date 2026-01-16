/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 13:06:17 by jotong            #+#    #+#             */
/*   Updated: 2026/01/16 13:08:30 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

char	**get_paths_from_env(char **envp)
{
	int		i;
	char	*path_value;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			path_value = envp[i] + 5;
			return (ft_split(path_value, ':'));
		}
		i++;
	}
	return (NULL);
}

int	setup_redir_in(t_ast *node, t_shell *shell)
{
	int	fd;
	int	source_fd;

	node->filename = expand_and_replace(&node->filename, shell);
	if (node->filename && node->filename[0] == '&'
		&& is_file_descriptor(node->filename + 1))
	{
		source_fd = ft_atoi(node->filename + 1);
		if (dup2(source_fd, node->fd) == -1)
			return (1);
		return (0);
	}
	fd = open(node->filename, O_RDONLY);
	if (fd == -1)
	{
		perror(node->filename);
		return (1);
	}
	dup2(fd, node->fd);
	close(fd);
	return (0);
}

int	setup_redir_out(t_ast *node, t_shell *shell)
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
	fd = open(node->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror(node->filename);
		return (1);
	}
	dup2(fd, node->fd);
	close(fd);
	return (0);
}

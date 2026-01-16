/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 13:11:21 by jotong            #+#    #+#             */
/*   Updated: 2026/01/16 13:13:45 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

int	is_builtin(t_ast *ast)
{
	int	av_len;

	if (!(ast || ast->argv || ast->argv[0]))
		return (0);
	av_len = ft_strlen(ast->argv[0]);
	if (ft_strncmp(ast->argv[0], "cd", av_len) == 0 && av_len == 2)
		return (1);
	if (ft_strncmp(ast->argv[0], "echo", av_len) == 0 && av_len == 4)
		return (1);
	if (ft_strncmp(ast->argv[0], "env", av_len) == 0 && av_len == 3)
		return (1);
	if (ft_strncmp(ast->argv[0], "exit", av_len) == 0 && av_len == 4)
		return (1);
	if (ft_strncmp(ast->argv[0], "export", av_len) == 0 && av_len == 6)
		return (1);
	if (ft_strncmp(ast->argv[0], "unset", av_len) == 0 && av_len == 5)
		return (1);
	if (ft_strncmp(ast->argv[0], "pwd", av_len) == 0 && av_len == 3)
		return (1);
	return (0);
}

int	execute_builtin(t_ast *ast, t_shell *shell)
{
	char	*cmd;
	int		arg_len;
	int		result;

	if (!ast || !ast->argv || !ast->argv[0])
		return (1);
	cmd = ast->argv[0];
	arg_len = ft_strlen(cmd);
	result = exec_builtin_part1(cmd, arg_len, ast, shell);
	if (result != -100)
		return (result);
	result = exec_builtin_part2(cmd, arg_len, ast, shell);
	return (result);
}

char	*find_command_path(char *cmd, char **envp)
{
	char	**paths;
	char	*full_path;
	char	*temp;
	int		i;

	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	paths = get_paths_from_env(envp);
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(temp, cmd);
		free(temp);
		if (access(full_path, X_OK) == 0)
			return (free_array(paths), full_path);
		free(full_path);
		i++;
	}
	free_array(paths);
	return (NULL);
}

int	execute_external(t_ast *ast, t_shell *shell)
{
	pid_t	pid;
	int		status;

	status = 0;
	pid = fork();
	if (pid == -1)
		return (1);
	if (pid == 0)
		execute_external_child(ast, shell);
	waitpid(pid, &status, 0);
	return (handle_wait_status(status));
}

int	execute_cmd(t_ast *node, t_shell *shell)
{
	int	status;
	int	i;

	i = 0;
	if (!node->argv || !node->argv[0])
		return (0);
	i = 0;
	while (node->argv[i])
	{
		expand_and_replace(&(node->argv[i]), shell);
		i++;
	}
	if (is_builtin(node))
		status = execute_builtin(node, shell);
	else
		status = execute_external(node, shell);
	return (status);
}

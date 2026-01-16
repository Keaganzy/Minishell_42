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

	if (!ast || !ast->argv || !ast->argv[0])
		return (1);
	cmd = ast->argv[0];
	arg_len = ft_strlen(cmd);
	if (ft_strncmp(cmd, "cd", arg_len) == 0 && ft_strlen(ast->argv[0]) == 2)
		return (builtin_cd(ast->argv, shell));
	else if (ft_strncmp(cmd, "echo", arg_len) == 0 && ft_strlen(ast->argv[0]) == 4)
		return (ft_echo(ast->argv, shell));
	else if (ft_strncmp(cmd, "env", arg_len) == 0 && ft_strlen(ast->argv[0]) == 3)
		return (ft_env(ast->argv, shell));
	else if (ft_strncmp(cmd, "pwd", arg_len) == 0 && ft_strlen(ast->argv[0]) == 3)
		return (ft_pwd(ast->argv, shell));
	else if (ft_strncmp(cmd, "exit", arg_len) == 0 && ft_strlen(ast->argv[0]) == 4)
		return (ft_exit(ast->argv, shell));
	else if (ft_strncmp(cmd, "export", arg_len) == 0 && ft_strlen(ast->argv[0]) == 6)
		return (ft_export(ast->argv, shell));
	else if (ft_strncmp(cmd, "unset", arg_len) == 0 && ft_strlen(ast->argv[0]) == 5)
		return (ft_unset(ast->argv, shell));
	return (-100);
}

static char	*find_command_path(char *cmd, char **envp)
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

static int	execute_external(t_ast *ast, t_shell *shell)
{
	pid_t	pid;
	int		status;
	char	*cmd_path;
	int		sig;

	status = 0;
	pid = fork();
	if (pid == -1)
		return (1);
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		cmd_path = find_command_path(ast->argv[0], shell->envp);
		if (!cmd_path)
		{
			ft_putstr_fd(ast->argv[0], 2);
			ft_putstr_fd(": command not found\n", 2);
			cleanup_shell(shell);
			exit(127);
		}
		execve(cmd_path, ast->argv, shell->envp);
		perror(ast->argv[0]);
		free(cmd_path);
		cleanup_shell(shell);
		exit(126);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGINT)
			return (130);
		if (sig == 3)
			write(1, "(Core Dumped)\n", 15);
		return (128 + sig);
	}
	return (1);
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

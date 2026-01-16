/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 15:44:48 by ksng              #+#    #+#             */
/*   Updated: 2026/01/16 10:18:10 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

int	execute_node(t_ast *node, t_shell *shell);
int	execute_cmd(t_ast *node, t_shell *shell);
int	execute_pipe(t_ast *node, t_shell *shell);
int	execute_redir(t_ast *node, t_shell *shell);
int	execute_logical(t_ast *node, t_shell *shell);

/* ************************************************************************** */
/*                           MAIN EXECUTOR                                    */
/* ************************************************************************** */

// int	execute_ast(t_ast *curr, t_shell *shell)
// {
// 	// int		prev_fd;
// 	int	status;

// 	if (!curr)
// 		return (0);
// 	status = execute_node(curr, shell);
// 	shell->last_exit_status = status;
// 	return (status);
// }

// int	execute_node(t_ast *node, t_shell *shell)
// {
// 	if (!node)
// 		return (0);
// 	if (node->type == N_CMD)
// 		return (execute_cmd(node, shell));
// 	if (node->type == N_PIPE)
// 		return (execute_pipe(node, shell));
// 	if (node->type == N_REDIR_IN || node->type == N_REDIR_OUT
// 		|| node->type == N_REDIR_APPEND || node->type == N_HEREDOC)
// 		return (execute_redir(node, shell));
// 	if (node->type == N_AND || node->type == N_OR)
// 		return (execute_logical(node, shell));
// 	return (1);
// }

/* ************************************************************************** */
/*                           LOGICAL OPERATORS (&&, ||)                       */
/* ************************************************************************** */

// int	execute_logical(t_ast *node, t_shell *shell)
// {
// 	int	left_status;

// 	left_status = execute_node(node->left, shell);
// 	shell->last_exit_status = left_status;
// 	if (node->type == N_AND)
// 	{
// 		if (left_status == 0)
// 			return (execute_node(node->right, shell));
// 		return (left_status);
// 	}
// 	if (node->type == N_OR)
// 	{
// 		if (left_status != 0)
// 			return (execute_node(node->right, shell));
// 		return (left_status);
// 	}
// 	return (1);
// }

/* ************************************************************************** */
/*                           PIPE EXECUTION                                   */
/* ************************************************************************** */

// int	execute_pipe_child(t_ast *node, int *pipefd,
// 		t_shell *shell, int is_left)
// {
// 	if (is_left)
// 	{
// 		close(pipefd[0]);
// 		dup2(pipefd[1], STDOUT_FILENO);
// 		close(pipefd[1]);
// 		exit(execute_node(node->left, shell));
// 	}
// 	else
// 	{
// 		close(pipefd[1]);
// 		dup2(pipefd[0], STDIN_FILENO);
// 		close(pipefd[0]);
// 		exit(execute_node(node->right, shell));
// 	}
// }

// int	wait_for_children(pid_t pid1, pid_t pid2)
// {
// 	int	status1;
// 	int	status2;
// 	int	final_status;

// 	waitpid(pid1, &status1, 0);
// 	waitpid(pid2, &status2, 0);
// 	if (WIFEXITED(status2))
// 		final_status = WEXITSTATUS(status2);
// 	else
// 		final_status = 1;
// 	return (final_status);
// }

// int	execute_pipe(t_ast *node, t_shell *shell)
// {
// 	int		pipefd[2];
// 	pid_t	pid1;
// 	pid_t	pid2;

// 	if (pipe(pipefd) == -1)
// 		return (1);
// 	pid1 = fork();
// 	if (pid1 == -1)
// 		return (close(pipefd[0]), close(pipefd[1]), 1);
// 	if (pid1 == 0)
// 		execute_pipe_child(node, pipefd, shell, 1);
// 	pid2 = fork();
// 	if (pid2 == -1)
// 		return (close(pipefd[0]), close(pipefd[1]), 1);
// 	if (pid2 == 0)
// 		execute_pipe_child(node, pipefd, shell, 0);
// 	close(pipefd[0]);
// 	close(pipefd[1]);
// 	return (wait_for_children(pid1, pid2));
// }

/* ************************************************************************** */
/*                           REDIRECTION EXECUTION                            */
/* ************************************************************************** */

// int	setup_redir_in(char *filename)
// {
// 	int	fd;

// 	fd = open(filename, O_RDONLY);
// 	if (fd == -1)
// 	{
// 		//perror(filename);
// 		return (1);
// 	}
// 	dup2(fd, STDIN_FILENO);
// 	close(fd);
// 	return (0);
// }

// int	setup_redir_out(char *filename)
// {
// 	int	fd;

// 	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 	if (fd == -1)
// 	{
// 		//perror(filename);
// 		return (1);
// 	}
// 	dup2(fd, STDOUT_FILENO);
// 	close(fd);
// 	return (0);
// }

// int	setup_redir_append(char *filename)
// {
// 	int	fd;

// 	fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
// 	if (fd == -1)
// 	{
// 		//perror(filename);
// 		return (1);
// 	}
// 	dup2(fd, STDOUT_FILENO);
// 	close(fd);
// 	return (0);
// }

// int	setup_heredoc(char *delimiter, t_shell *shell)
// {
// 	int		pipefd[2];
// 	char	*line;

// 	if (pipe(pipefd) == -1)
// 		return (1);
// 	while (!shell->exit_code)
// 	{
// 		line = readline("> ");
// 		if (!line || ft_strcmp(line, delimiter) == 0)
// 		{
// 			free(line);
// 			break ;
// 		}
// 		write(pipefd[1], line, ft_strlen(line));
// 		write(pipefd[1], "\n", 1);
// 		free(line);
// 	}
// 	close(pipefd[1]);
// 	dup2(pipefd[0], STDIN_FILENO);
// 	close(pipefd[0]);
// 	return (0);
// }

// int	setup_redirection(t_ast *node, t_shell *shell)
// {
// 	if (node->type == N_REDIR_IN)
// 		return (setup_redir_in(node->filename));
// 	if (node->type == N_REDIR_OUT)
// 		return (setup_redir_out(node->filename));
// 	if (node->type == N_REDIR_APPEND)
// 		return (setup_redir_append(node->filename));
// 	if (node->type == N_HEREDOC)
// 		return (setup_heredoc(node->filename, shell));
// 	return (1);
// }

// int	execute_redir(t_ast *node, t_shell *shell)
// {
// 	int	status;

// 	backup_fds(shell);
// 	status = setup_redirection(node, shell);
// 	if (status != 0)
// 	{
// 		restore_fds(shell);
// 		return (status);
// 	}
// 	status = execute_node(node->left, shell);
// 	restore_fds(shell);
// 	return (status);
// }

/* ************************************************************************** */
/*                           COMMAND EXECUTION                                */
/* ************************************************************************** */


// int is_builtin(t_ast *ast)
// {
// 	int	av_len;

// 	if (!(ast || ast->argv || ast->argv[0]))
// 		return (0);
// 	av_len = ft_strlen(ast->argv[0]);
// 	if (ft_strncmp(ast->argv[0], "cd", av_len) == 0 && av_len == 2)
// 		return (1);
// 	if (ft_strncmp(ast->argv[0], "echo", av_len) == 0 && av_len == 4)
// 		return (1);
// 	if (ft_strncmp(ast->argv[0], "env", av_len) == 0 && av_len == 3)
// 		return (1);
// 	if (ft_strncmp(ast->argv[0], "exit", av_len) == 0 && av_len == 4)
// 		return (1);
// 	if (ft_strncmp(ast->argv[0], "export", av_len) == 0 && av_len == 6)
// 		return (1);
// 	if (ft_strncmp(ast->argv[0], "unset", av_len) == 0 && av_len == 5)
// 		return (1);
// 	if (ft_strncmp(ast->argv[0], "pwd", av_len) == 0 && av_len == 3)
// 		return (1);
// 	return (0);
// }

// int	execute_builtin(t_ast *ast, t_shell *shell)
// {
// 	char	*cmd;
// 	int		arg_len;

// 	printf("execute builtin called\n");
// 	if (!ast || !ast->argv || !ast->argv[0])
// 		return (1);
// 	cmd = ast->argv[0];
// 	arg_len = ft_strlen(cmd);
// 	if (ft_strncmp(cmd, "cd", arg_len) == 0 && ft_strlen(ast->argv[0]) == 2)
// 		return (builtin_cd(ast->argv, shell));
// 	else if (ft_strncmp(cmd, "echo", arg_len) == 0 && ft_strlen(ast->argv[0]) == 4)
// 		return (ft_echo(ast->argv, shell));
// 	else if (ft_strncmp(cmd, "env", arg_len) == 0 && ft_strlen(ast->argv[0]) == 3)
// 		return (ft_env(ast->argv, shell));
// 	else if (ft_strncmp(cmd, "pwd", arg_len) == 0 && ft_strlen(ast->argv[0]) == 3)
// 		return (ft_pwd(ast->argv, shell));
// 	else if (ft_strncmp(cmd, "exit", arg_len) == 0 && ft_strlen(ast->argv[0]) == 4)
// 		return (ft_exit(ast->argv, shell));
// 	else if (ft_strncmp(cmd, "export", arg_len) == 0 && ft_strlen(ast->argv[0]) == 6)
// 		return (ft_export(ast->argv, shell));
// 	else if (ft_strncmp(cmd, "unset", arg_len) == 0 && ft_strlen(ast->argv[0]) == 5)
// 		return(ft_unset(ast->argv, shell));
// 	return (-100);
// }

// static char	*find_command_path(char *cmd, char **envp)
// {
// 	char	**paths;
// 	char	*full_path;
// 	char	*temp;
// 	int		i;

// 	if (ft_strchr(cmd, '/'))
// 		return (ft_strdup(cmd));
// 	paths = get_paths_from_env(envp);
// 	if (!paths)
// 		return (NULL);
// 	i = 0;
// 	while (paths[i])
// 	{
// 		temp = ft_strjoin(paths[i], "/");
// 		full_path = ft_strjoin(temp, cmd);
// 		free(temp);
// 		if (access(full_path, X_OK) == 0)
// 			return (free_array(paths), full_path);
// 		free(full_path);
// 		i++;
// 	}
// 	free_array(paths);
// 	return (NULL);
// }

// static int	execute_external(t_ast *ast, t_shell *shell)
// {
// 	pid_t	pid;
// 	int		status;
// 	char	*cmd_path;

// 	pid = fork();
// 	if (pid == -1)
// 		return (1);
// 	if (pid == 0)
// 	{
// 		cmd_path = find_command_path(ast->argv[0], shell->envp);
// 		if (!cmd_path)
// 		{
// 			ft_putstr_fd(ast->argv[0], 2);
// 			ft_putstr_fd(": command not found\n", 2);
// 			exit(127);
// 		}
// 		execve(cmd_path, ast->argv, shell->envp);
// 		perror(cmd_path);
// 		free(cmd_path);
// 		exit(126);
// 	}
// 	waitpid(pid, &status, 0);
// 	if (WIFEXITED(status))
// 		return (WEXITSTATUS(status));
// 	return (1);
// }

// int	execute_cmd(t_ast *node, t_shell *shell)
// {
// 	int	status;

// 	if (!node->argv || !node->argv[0])
// 		return (0);
// 	if (is_builtin(node))
// 		status = execute_builtin(node, shell);
// 	else
// 		status = execute_external(node, shell);
// 	shell->last_exit_status = status;
// 	return (status);
// }

/* ************************************************************************** */
/*                           HELPER: PATH EXTRACTION                          */
/* ************************************************************************** */

// char	**get_paths_from_env(char **envp)
// {
// 	int		i;
// 	char	*path_value;

// 	i = 0;
// 	while (envp[i])
// 	{
// 		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
// 		{
// 			path_value = envp[i] + 5;
// 			return (ft_split(path_value, ':'));
// 		}
// 		i++;
// 	}
// 	return (NULL);
// }

/* ************************************************************************** */
/*                           HELPER: FREE ARRAY                               */
/* ************************************************************************** */

// void	free_array(char **arr)
// {
// 	int	i;

// 	if (!arr)
// 		return ;
// 	i = 0;
// 	while (arr[i])
// 	{
// 		free(arr[i]);
// 		i++;
// 	}
// 	free(arr);
// }

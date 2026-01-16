#include "minishell.h"
#include "libft.h"

int	exec_builtin_part1(char *cmd, int len, t_ast *ast, t_shell *shell)
{
	if (ft_strncmp(cmd, "cd", len) == 0 && len == 2)
		return (builtin_cd(ast->argv, shell));
	else if (ft_strncmp(cmd, "echo", len) == 0 && len == 4)
		return (ft_echo(ast->argv, shell));
	else if (ft_strncmp(cmd, "env", len) == 0 && len == 3)
		return (ft_env(ast->argv, shell));
	else if (ft_strncmp(cmd, "pwd", len) == 0 && len == 3)
		return (ft_pwd(ast->argv, shell));
	return (-100);
}

int	exec_builtin_part2(char *cmd, int len, t_ast *ast, t_shell *shell)
{
	if (ft_strncmp(cmd, "exit", len) == 0 && len == 4)
		return (ft_exit(ast->argv, shell));
	else if (ft_strncmp(cmd, "export", len) == 0 && len == 6)
		return (ft_export(ast->argv, shell));
	else if (ft_strncmp(cmd, "unset", len) == 0 && len == 5)
		return (ft_unset(ast->argv, shell));
	return (-100);
}

void	execute_external_child(t_ast *ast, t_shell *shell)
{
	char	*cmd_path;

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

int	handle_wait_status(int status)
{
	int	sig;

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
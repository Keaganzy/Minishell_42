#include "minishell.h"
#include "libft.h"

static void	heredoc_child_process(int pipefd[2], char *delimiter)
{
	char	*content;

	close(pipefd[0]);
	content = read_heredoc_content(delimiter);
	if (content)
	{
		write(pipefd[1], content, ft_strlen(content));
		free(content);
	}
	close(pipefd[1]);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	exit(0);
}

static int	wait_heredoc_child(pid_t pid, int pipefd[2])
{
	struct sigaction	sa_old;
	int					status;

	close(pipefd[1]);
	sigaction(SIGINT, NULL, &sa_old);
	signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	sigaction(SIGINT, &sa_old, NULL);
	if (WIFEXITED(status) && WEXITSTATUS(status) == 1)
	{
		close(pipefd[0]);
		g_sigint_received = 130;
		return (0);
	}
	return (1);
}

static char	*read_heredoc_pipe(int pipefd[2])
{
	char	*heredoc_content;
	char	buffer[4096];
	ssize_t	n;
	char	*tmp;

	heredoc_content = ft_strdup("");
	n = read(pipefd[0], buffer, sizeof(buffer) - 1);
	while (n > 0)
	{
		buffer[n] = '\0';
		tmp = heredoc_content;
		heredoc_content = ft_strjoin(heredoc_content, buffer);
		free(tmp);
		n = read(pipefd[0], buffer, sizeof(buffer) - 1);
	}
	close(pipefd[0]);
	return (heredoc_content);
}

static t_ast	*setup_heredoc_node(t_ast *node, char *delimiter,
		t_shell *shell, int flag)
{
	pid_t	pid;
	int		pipefd[2];

	if (pipe(pipefd) == -1)
		return (free_ast(node), NULL);
	pid = fork();
	if (pid == -1)
		return (close(pipefd[0]), close(pipefd[1]), free_ast(node), NULL);
	if (pid == 0)
		heredoc_child_process(pipefd, delimiter);
	if (!wait_heredoc_child(pid, pipefd))
		return (free_ast(node), NULL);
	node->heredoc_content = read_heredoc_pipe(pipefd);
	if (!node->heredoc_content && g_sigint_received)
		return (free_ast(node), cleanup_shell(shell), NULL);
	if (!flag && node->heredoc_content)
		node->heredoc_content = expand_heredoc_line(node->heredoc_content,
				shell);
	return (node);
}

t_ast	*handle_heredoc(t_node_type type, t_token *file_token,
		t_ast *cmd, t_shell *shell)
{
	t_ast	*node;
	char	*old_value;
	int		flag;
	char	*delimiter;

	flag = 0;
	old_value = file_token->value;
	file_token->value = strip_quotes(file_token->value, &flag);
	free(old_value);
	delimiter = file_token->value;
	node = create_redir_node(type, delimiter, cmd, 0);
	if (!node)
		return (NULL);
	return (setup_heredoc_node(node, delimiter, shell, flag));
}
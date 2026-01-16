/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd_redirect_new.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 12:48:32 by jotong            #+#    #+#             */
/*   Updated: 2026/01/16 12:51:49 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

t_ast   *parser_word(t_parser *p)
{
	t_ast	*node;
	int		word_count;
	int		i;

	word_count = count_words(p);
	if (word_count == 0)
		return (NULL);
	node = create_node(N_CMD);
	if (!node)
		return (NULL);
	node->argv = malloc(sizeof(char *) * (word_count + 1));
	if (!node->argv)
		return (free(node), NULL);
	i = 0;
	while (i < word_count)
	{
		node->argv[i] = p->current->value;
		p->current->value = NULL;
		if (!node->argv[i])
			return (free_ast(node), NULL);
		advance(p);
		i++;
	}
	node->argv[i] = NULL;
	return (node);
}

t_ast	*parse_command(t_parser *p, t_shell *shell)
{
	t_ast	*node;

	if (match(p, T_OPEN_BRACKET))
	{
		advance(p);
		node = parse_logical(p, shell);
		if (!node)
			return (NULL);
		if (!expect(p, T_CLOSE_BRACKET))
		{
			free_ast(node);
			return (NULL);
		}
		return (node);
	}
	if (match(p, T_WORD))
		return (parser_word(p));
	return (NULL);
}

char	*strip_quotes(const char *str, int *flag)
{
	char	*result;
	int		i;
	int		j;
	char	quote;

	i = 0;
	j = 0;
	result = malloc(strlen(str) + 1);
	if (!result)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '"' || str[i] == '\'')
		{
			quote = str[i];
			*flag = 1;
			i++;
			while (str[i] && str[i] != quote)
				result[j++] = str[i++];
			if (str[i] == quote)
				i++;
		}
		else
			result[j++] = str[i++];
	}
	result[j] = '\0';
	return (result);
}

t_ast   *parse_one_redir(t_parser *p, t_ast *cmd, t_shell *shell)
{
	t_token		*redir_token;
	t_token		*file_token;
	t_node_type redir_type;
	t_ast		*node;
	int			flag;
	int			fd;
	char		*old_value;
	pid_t		pid;
	int			status;
	int			pipefd[2];
	char		*heredoc_content;
	char		buffer[4096];
	ssize_t		n;
	char		*tmp;
	struct sigaction	sa_old;

	status = 0;
	flag = 0;
	if (g_sigint_received == 130)
	{
		free_ast(cmd);
		return (NULL);
	}
	redir_token = advance(p);
	redir_type = get_redir_type(redir_token->type);
	fd = redir_token->fd;
	if (!peek(p))
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", 2);
		shell->last_exit_status = 2;
		free_ast(cmd);
		return (NULL);
	}
	if (is_redirection(peek(p)->type) || peek(p)->type == T_PIPE ||
		peek(p)->type == T_AND || peek(p)->type == T_OR)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token\n", 2);
		shell->last_exit_status = 2;
		free_ast(cmd);
		return (NULL);
	}
	file_token = expect(p, T_WORD);
	if (!file_token)
	{
		ft_putstr_fd("minishell: syntax error near redirect\n", 2);
		shell->last_exit_status = 2;
		free_ast(cmd);
		return (NULL);
	}
	if (redir_type == N_HEREDOC)
	{
		old_value = file_token->value;
		file_token->value = strip_quotes(file_token->value, &flag);
		free(old_value);
		node = create_redir_node(redir_type, file_token->value, cmd, fd);
		if (node)
		{
			if (pipe(pipefd) == -1)
			{
				free_ast(node);
				return (NULL);
			}
			pid = fork();
			if (pid == -1)
			{
				close(pipefd[0]);
				close(pipefd[1]);
				free_ast(node);
				return (NULL);
			}
			if (pid == 0)
			{
				close(pipefd[0]);
				char *content = read_heredoc_content(file_token->value);
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
			close(pipefd[1]);
			sigaction(SIGINT, NULL, &sa_old);
			signal(SIGINT, SIG_IGN);
			waitpid(pid, &status, 0);
			sigaction(SIGINT, &sa_old, NULL);
			if (WIFEXITED(status) && WEXITSTATUS(status) == 1)
			{
				close(pipefd[0]);
				free_ast(node);
				g_sigint_received = 130;
				return (NULL);
			}
			heredoc_content = ft_strdup("");
			while ((n = read(pipefd[0], buffer, sizeof(buffer) - 1)) > 0)
			{
				buffer[n] = '\0';
				tmp = heredoc_content;
				heredoc_content = ft_strjoin(heredoc_content, buffer);
				free(tmp);
			}
			close(pipefd[0]);
			node->heredoc_content = heredoc_content;
			if (!node->heredoc_content && g_sigint_received)
			{
				free_ast(node);
				cleanup_shell(shell);
				return (NULL);
			}
			if (!flag && node->heredoc_content)
				node->heredoc_content = expand_heredoc_line(node->heredoc_content, shell);
		}
		return (node);
	}
	return (create_redir_node(redir_type, file_token->value, cmd, fd));
}

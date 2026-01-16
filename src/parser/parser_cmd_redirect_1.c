#include "minishell.h"
#include "libft.h"

static void	copy_quoted_content(const char *str, char *result, int *i, int *j)
{
	char	quote;

	quote = str[*i];
	(*i)++;
	while (str[*i] && str[*i] != quote)
		result[(*j)++] = str[(*i)++];
	if (str[*i] == quote)
		(*i)++;
}

char	*strip_quotes(const char *str, int *flag)
{
	char	*result;
	int		i;
	int		j;

	i = 0;
	j = 0;
	result = malloc(ft_strlen(str) + 1);
	if (!result)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '"' || str[i] == '\'')
		{
			*flag = 1;
			copy_quoted_content(str, result, &i, &j);
		}
		else
			result[j++] = str[i++];
	}
	result[j] = '\0';
	return (result);
}

int	validate_redir_token(t_parser *p, t_ast *cmd, t_shell *shell)
{
	if (!peek(p))
	{
		ft_putstr_fd("minishell: syntax error near unexpected token"
			" `newline'\n", 2);
		shell->last_exit_status = 2;
		free_ast(cmd);
		return (0);
	}
	if (is_redirection(peek(p)->type) || peek(p)->type == T_PIPE
		|| peek(p)->type == T_AND || peek(p)->type == T_OR)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token\n", 2);
		shell->last_exit_status = 2;
		free_ast(cmd);
		return (0);
	}
	return (1);
}

t_token	*get_file_token(t_parser *p, t_ast *cmd, t_shell *shell)
{
	t_token	*file_token;

	file_token = expect(p, T_WORD);
	if (!file_token)
	{
		ft_putstr_fd("minishell: syntax error near redirect\n", 2);
		shell->last_exit_status = 2;
		free_ast(cmd);
		return (NULL);
	}
	return (file_token);
}

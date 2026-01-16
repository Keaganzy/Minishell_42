#include "minishell.h"
#include "libft.h"

t_ast	*parse_one_redir(t_parser *p, t_ast *cmd, t_shell *shell)
{
	t_token			*redir_token;
	t_token			*file_token;
	t_node_type		redir_type;
	int				fd;

	if (g_sigint_received == 130)
		return (free_ast(cmd), NULL);
	redir_token = advance(p);
	redir_type = get_redir_type(redir_token->type);
	fd = redir_token->fd;
	if (!validate_redir_token(p, cmd, shell))
		return (NULL);
	file_token = get_file_token(p, cmd, shell);
	if (!file_token)
		return (NULL);
	if (redir_type == N_HEREDOC)
		return (handle_heredoc(redir_type, file_token, cmd, shell));
	return (create_redir_node(redir_type, file_token->value, cmd, fd));
}
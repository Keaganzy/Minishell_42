/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 18:04:47 by jotong            #+#    #+#             */
/*   Updated: 2026/01/16 18:04:48 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

t_ast	*find_bottom_left(t_ast *cmd)
{
	t_ast	*bottom;

	bottom = cmd;
	while (bottom->left)
		bottom = bottom->left;
	return (bottom);
}

t_ast	*attach_command_to_redir(t_parser *p, t_ast *cmd,
				t_shell *shell)
{
	t_ast	*bottom;
	t_ast	*parsed_cmd;

	bottom = find_bottom_left(cmd);
	parsed_cmd = parse_command(p, shell);
	if (!parsed_cmd)
		return (free_ast(cmd), NULL);
	bottom->left = parsed_cmd;
	return (cmd);
}

t_ast	*handle_command_parse(t_parser *p, t_ast *cmd,
				t_shell *shell)
{
	if (cmd)
		return (attach_command_to_redir(p, cmd, shell));
	else
	{
		cmd = parse_command(p, shell);
		if (!cmd)
			return (NULL);
		return (cmd);
	}
}

t_ast	*handle_no_command(t_ast *cmd, t_shell *shell)
{
	free_ast(cmd);
	ft_putstr_fd(
		"minishell: syntax error near unexpected token `newline'\n", 2);
	shell->last_exit_status = 2;
	return (NULL);
}

t_ast	*parse_redirection(t_parser *p, t_shell *shell)
{
	t_ast	*cmd;

	cmd = parse_prefix_redirections(p, shell);
	if (peek(p) && (match(p, T_WORD) || match(p, T_OPEN_BRACKET)))
	{
		cmd = handle_command_parse(p, cmd, shell);
		if (!cmd)
			return (NULL);
	}
	else if (!cmd)
		return (handle_no_command(cmd, shell));
	cmd = parse_suffix_redirections(p, cmd, shell);
	return (cmd);
}

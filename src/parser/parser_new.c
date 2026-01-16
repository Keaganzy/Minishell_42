/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_new.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 12:42:01 by jotong            #+#    #+#             */
/*   Updated: 2026/01/16 12:45:09 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

static t_ast	*parse_pipe(t_parser *p, t_shell *shell);
static t_ast	*parse_redirection(t_parser *p, t_shell *shell);

t_ast	*parse_logical(t_parser *p, t_shell *shell)
{
	t_ast		*left;
	t_ast		*right;
	t_node_type	op;

	left = parse_pipe(p, shell);
	if (!left)
		return (NULL);
	while (match(p, T_AND) || match(p, T_OR))
	{
		if (match(p, T_AND))
			op = N_AND;
		else
			op = N_OR;
		advance(p);
		right = parse_pipe(p, shell);
		if (!right)
			return (free_ast(left), NULL);
		left = create_binary_node(op, left, right);
		if (!left)
			return (free_ast(right), NULL);
	}
	return (left);
}

static t_ast	*parse_pipe(t_parser *p, t_shell *shell)
{
	t_ast	*left;
	t_ast	*right;

	left = parse_redirection(p, shell);
	if (!left)
		return (NULL);
	while (match(p, T_PIPE))
	{
		advance(p);
		right = parse_redirection(p, shell);
		if (!right)
			return (free_ast(left), NULL);
		left = create_binary_node(N_PIPE, left, right);
		if (!left)
			return (free_ast(right), NULL);
	}
	return (left);
}

static t_ast	*find_bottom_left(t_ast *cmd)
{
	t_ast	*bottom;

	bottom = cmd;
	while (bottom->left)
		bottom = bottom->left;
	return (bottom);
}

static t_ast	*parse_prefix_redirections(t_parser *p, t_shell *shell)
{
	t_ast	*cmd;

	cmd = NULL;
	while (peek(p) && is_redirection(peek(p)->type))
	{
		cmd = parse_one_redir(p, cmd, shell);
		if (!cmd)
			return (NULL);
	}
	return (cmd);
}

static t_ast	*attach_command_to_redir(t_parser *p, t_ast *cmd,
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

static t_ast	*handle_command_parse(t_parser *p, t_ast *cmd, t_shell *shell)
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

static t_ast	*handle_no_command(t_ast *cmd, t_shell *shell)
{
	free_ast(cmd);
	ft_putstr_fd(
		"minishell: syntax error near unexpected token `newline'\n", 2);
	shell->last_exit_status = 2;
	return (NULL);
}

static t_ast	*parse_suffix_redirections(t_parser *p, t_ast *cmd,
					t_shell *shell)
{
	while (peek(p) && is_redirection(peek(p)->type))
	{
		cmd = parse_one_redir(p, cmd, shell);
		if (!cmd)
			return (NULL);
	}
	return (cmd);
}

static t_ast	*parse_redirection(t_parser *p, t_shell *shell)
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

t_ast	*parse(t_token *tokens, t_shell *shell)
{
	t_parser	parser;
	t_ast		*ast;

	parser.current = tokens;
	skip_spaces(&parser);
	ast = parse_logical(&parser, shell);
	if (!ast)
		return (NULL);
	skip_spaces(&parser);
	if (parser.current != NULL)
	{
		free_ast(ast);
		return (NULL);
	}
	return (ast);
}

/* ========================================================================== */
/* OPERATOR PRECEDENCE STRUCTURE (CORRECT):								  */
/*																			*/
/* parse()																	*/
/*   └─ parse_logical() - handles && and || at SAME level, left-to-right	 */
/*	   └─ parse_pipe() - handles | (higher precedence than && ||)		  */
/*		   └─ parse_redirection() - handles >, <, >> (highest precedence)  */
/*			   └─ parse_command() - actual command						 */
/*																			*/
/* This ensures:															 */
/* 1. && and || have equal precedence										*/
/* 2. Both are left-associative											  */
/* 3. "true || false && false" evaluates as "(true || false) && false"	   */
/*	which is correct: "true || false" = true, "true && false" = false	  */
/* ========================================================================== */

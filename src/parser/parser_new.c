/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_new.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksng <ksng@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 15:20:42 by ksng              #+#    #+#             */
/*   Updated: 2026/01/13 17:28:56 by ksng             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

static t_ast	*parse_pipe(t_parser *p, t_shell *shell);
static t_ast	*parse_redirection(t_parser *p, t_shell *shell);

/* ========================================================================== */
/* FIXED: parse_logical handles both && and || at same level, left-to-right  */
/* This ensures operators have equal precedence and evaluate left-associative */
/* ========================================================================== */
t_ast	*parse_logical(t_parser *p, t_shell *shell)
{
	t_ast	*left;
	t_ast	*right;
	t_node_type op;

	left = parse_pipe(p, shell);
	if (!left)
		return (NULL);

	/* Handle both && and || in the same loop for left-to-right evaluation */
	while (match(p, T_AND) || match(p, T_OR))
	{
		/* Determine which operator we have */
		if (match(p, T_AND))
			op = N_AND;
		else
			op = N_OR;

		advance(p);

		/* Parse the right side (just pipe level, not another logical op) */
		right = parse_pipe(p, shell);
		if (!right)
		{
			free_ast(left);
			return (NULL);
		}

		/* Create the node and update left for next iteration */
		left = create_binary_node(op, left, right);
		if (!left)
		{
			free_ast(right);
			return (NULL);
		}
	}

	return (left);
}

static t_ast	*parse_pipe(t_parser *p, t_shell *shell)
{
	t_ast *left;
	t_ast *right;

	left = parse_redirection(p, shell);
	if (!left)
		return (NULL);
	while (match(p, T_PIPE))
	{
		advance(p);
		right = parse_redirection(p, shell);
		if (!right)
		{
			free_ast(left);
			return (NULL);
		}
		left = create_binary_node(N_PIPE, left, right);
		if (!left)
		{
			free_ast(right);
			return (NULL);
		}
	}
	return (left);
}

static t_ast *parse_redirection(t_parser *p, t_shell *shell)
{
	t_ast *cmd;
    t_ast *bottom;

    cmd = NULL;

    // Parse redirections BEFORE command
    while (peek(p) && is_redirection(peek(p)->type))
    {
        cmd = parse_one_redir(p, cmd, shell);
        if (!cmd)
            return (NULL);
    }

    // Parse the command
    if (peek(p) && (match(p, T_WORD) || match(p, T_OPEN_BRACKET)))
    {
        if (cmd)
        {
            // Attach command at bottom of redir chain
            bottom = cmd;
            while (bottom->left)
                bottom = bottom->left;
            bottom->left = parse_command(p, shell);
            if (!bottom->left)
                return (free_ast(cmd), NULL);
        }
        else
        {
            // No redirections yet, just parse command
            cmd = parse_command(p, shell);
            if (!cmd)
                return (NULL);
        }
    }
    else if (!cmd)
    {
        // We have redirections but no command - ERROR!
        free_ast(cmd);
        // Print error message
        ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", 2);
        shell->last_exit_status = 2;
		return (NULL);
    }

    // Parse redirections AFTER command
    while (peek(p) && is_redirection(peek(p)->type))
    {
        cmd = parse_one_redir(p, cmd, shell);
        if (!cmd)
            return (NULL);
    }

    return (cmd);
}


t_ast *parse(t_token *tokens, t_shell *shell)
{
	t_parser	parser;
	t_ast	*ast;

	parser.current = tokens;
	//parser.tokens = tokens; Used for error reporting later
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
/* OPERATOR PRECEDENCE STRUCTURE (CORRECT):                                  */
/*                                                                            */
/* parse()                                                                    */
/*   └─ parse_logical() - handles && and || at SAME level, left-to-right     */
/*       └─ parse_pipe() - handles | (higher precedence than && ||)          */
/*           └─ parse_redirection() - handles >, <, >> (highest precedence)  */
/*               └─ parse_command() - actual command                         */
/*                                                                            */
/* This ensures:                                                             */
/* 1. && and || have equal precedence                                        */
/* 2. Both are left-associative                                              */
/* 3. "true || false && false" evaluates as "(true || false) && false"       */
/*    which is correct: "true || false" = true, "true && false" = false      */
/* ========================================================================== */



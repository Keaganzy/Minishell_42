// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   parser.c                                           :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: ksng <ksng@student.42.fr>                  +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2025/10/13 15:28:49 by ksng              #+#    #+#             */
// /*   Updated: 2025/11/14 17:32:22 by ksng             ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */


// #include "minishell.h"
// #include "libft.h"

// /* ************************************************************************** */
// /*                           PARSER UTILITIES                                 */
// /* ************************************************************************** */

// static void skip_spaces(t_parser *p)
// {
//     while (p->current && p->current->type == T_SPACE)
//         p->current = p->current->next;
// }

// static t_token	*peek(t_parser *p)
// {
// 	return (p->current);
// }

// static t_token	*advance(t_parser *p)
// {
// 	t_token	*tmp;

// 	tmp = p->current;
// 	if (p->current)
// 		p->current = p->current->next;

// 	skip_spaces(p);
// 	return (tmp);
// }

// static int	match(t_parser *p, t_token_type type)
// {
// 	if (!p->current)
// 		return (0);
// 	return (p->current->type == type);
// }

// static t_token	*expect(t_parser *p, t_token_type type)
// {
// 	if (!match(p, type))
// 		return (NULL);
// 	return (advance(p));
// }

// /* ************************************************************************** */
// /*                           NODE CONSTRUCTORS                                */
// /* ************************************************************************** */

// static t_ast	*create_node(t_node_type type)
// {
// 	t_ast	*node;

// 	node = malloc(sizeof(t_ast));
// 	if (!node)
// 		return (NULL);
// 	node->type = type;
// 	node->argv = NULL;
// 	node->filename = NULL;
// 	node->left = NULL;
// 	node->right = NULL;
// 	return (node);
// }

// static t_ast	*create_binary_node(t_node_type type, t_ast *left,
// 		t_ast *right)
// {
// 	t_ast	*node;

// 	node = create_node(type);
// 	if (!node)
// 		return (NULL);
// 	node->left = left;
// 	node->right = right;
// 	return (node);
// }



// static t_ast	*create_redir_node(t_node_type type, char *filename,
// 		t_ast *cmd)
// {
// 	t_ast	*node;

// 	node = create_node(type);
// 	if (!node)
// 		return (NULL);
// 	node->filename = ft_strdup(filename);
// 	if (!node->filename)
// 	{
// 		free(node);
// 		return (NULL);
// 	}
// 	node->left = cmd;
// 	return (node);
// }


// /* ************************************************************************** */
// /*                           WORD SEQUENCE PARSING                            */
// /* ************************************************************************** */

// static int	count_words(t_parser *p)
// {
// 	int		count;
// 	t_token	*tmp;

// 	count = 0;
// 	tmp = p->current;
// 	while (tmp)
//     {
//         // Skip spaces
//         while (tmp && tmp->type == T_SPACE)
//             tmp = tmp->next;

//         // Count word
//         if (tmp && tmp->type == T_WORD)
//         {
//             count++;
//             tmp = tmp->next;
//         }
//         else
//             break;
//     }
// 	return (count);
// }

// /* ************************************************************************** */
// /*                           FORWARD DECLARATIONS                             */
// /* ************************************************************************** */

// static t_ast	*parse_or(t_parser *p);
// static t_ast	*parse_and(t_parser *p);
// static t_ast	*parse_pipe(t_parser *p);
// static t_ast	*parse_command(t_parser *p);
// static t_ast	*parse_redirections(t_parser *p);
// static t_ast	*parse_simple_cmd(t_parser *p);
// static t_ast	*parse_primary(t_parser *p);

// /* ************************************************************************** */
// /*                    GRAMMAR IMPLEMENTATION (Top to Bottom)                  */
// /*                                                                            */
// /* Grammar:                                                                   */
// /*   or_expr    -> and_expr ( "||" and_expr )*                                */
// /*   and_expr   -> pipe_expr ( "&&" pipe_expr )*                              */
// /*   pipe_expr  -> command ( "|" command )*                                   */
// /*   command    -> redirections                                               */
// /*   redirections -> simple_cmd ( redir simple_cmd )*                         */
// /*   simple_cmd -> primary                                                    */
// /*   primary    -> "(" or_expr ")" | WORD+                                    */
// /* ************************************************************************** */

// /* Parse OR expressions (||) - lowest precedence */
// static t_ast	*parse_or(t_parser *p)
// {
// 	t_ast	*left;
// 	t_ast	*right;

// 	left = parse_and(p);
// 	// printf("address of left1: %p.", &left);
// 	if (!left)
// 		return (NULL);
// 	while (match(p, T_OR))
// 	{
// 		advance(p);
// 		right = parse_and(p);
// 		if (!right)
// 		{
// 			free_ast(left);
// 			return (NULL);
// 		}
// 		left = create_binary_node(N_OR, left, right);
// 		// printf("address of left: %p.", &left);
// 		if (!left)
// 		{
// 			free_ast(right);
// 			return (NULL);
// 		}
// 	}
// 	return (left);
// }

// /* Parse AND expressions (&&) */
// static t_ast	*parse_and(t_parser *p)
// {
// 	t_ast	*left;
// 	t_ast	*right;

// 	left = parse_pipe(p);
// 	if (!left)
// 		return (NULL);
// 	while (match(p, T_AND))
// 	{
// 		advance(p);
// 		right = parse_pipe(p);
// 		if (!right)
// 		{
// 			free_ast(left);
// 			return (NULL);
// 		}
// 		left = create_binary_node(N_AND, left, right);
// 		if (!left)
// 		{
// 			free_ast(right);
// 			return (NULL);
// 		}
// 	}
// 	return (left);
// }

// /* Parse PIPE expressions (|) */
// static t_ast	*parse_pipe(t_parser *p)
// {
// 	t_ast	*left;
// 	t_ast	*right;

// 	left = parse_command(p);
// 	if (!left)
// 		return (NULL);
// 	while (match(p, T_PIPE))
// 	{
// 		advance(p);
// 		right = parse_command(p);
// 		if (!right)
// 		{
// 			free_ast(left);
// 			return (NULL);
// 		}
// 		left = create_binary_node(N_PIPE, left, right);
// 		if (!left)
// 		{
// 			free_ast(right);
// 			return (NULL);
// 		}
// 	}
// 	return (left);
// }

// /* Parse command with redirections */
// static t_ast	*parse_command(t_parser *p)
// {
// 	return (parse_redirections(p));
// }

// /* ************************************************************************** */
// /*                           REDIRECTION PARSING                              */
// /* ************************************************************************** */

// static t_node_type	get_redir_type(t_token_type type)
// {
// 	if (type == T_REDIR_IN)
// 		return (N_REDIR_IN);
// 	if (type == T_REDIR_OUT)
// 		return (N_REDIR_OUT);
// 	if (type == T_REDIR_APPEND)
// 		return (N_REDIR_APPEND);
// 	if (type == T_HEREDOC)
// 		return (N_HEREDOC);
// 	return (N_CMD);
// }


// static t_ast	*parse_word_sequence(t_parser *p)
// {
// 	t_ast		*node;
// 	int			word_count;
// 	int			i;

// 	word_count = count_words(p);
// 	if (word_count == 0)
// 		return (NULL);
// 	node = create_node(N_CMD);
// 	if (!node)
// 		return (NULL);
// 	node->argv = malloc(sizeof(char *) * (word_count + 1));
// 	if (!node->argv)
// 		return (free(node), NULL);
// 	i = 0;
// 	while (i < word_count)
// 	{
// 		node->argv[i] = ft_strdup(p->current->value);
// 		if (!node->argv[i])
// 			return (free_ast(node), NULL);
// 		advance(p);
// 		i++;
// 	}
// 	node->argv[i] = NULL;
// 	return (node);
// }

// static int	is_redirection(t_token_type type)
// {
// 	return (type == T_REDIR_IN || type == T_REDIR_OUT
// 		|| type == T_REDIR_APPEND || type == T_HEREDOC);
// }

// static t_ast	*parse_single_redir(t_parser *p, t_ast *cmd)
// {
// 	t_token		*redir_tok;
// 	t_token		*file_tok;
// 	t_node_type	redir_type;

// 	redir_tok = advance(p); // on after redirect
// 	redir_type = get_redir_type(redir_tok->type);
// 	file_tok = expect(p, T_WORD);
// 	if (!file_tok)
// 	{
// 		free_ast(cmd);
// 		return (NULL);
// 	}
// 	return (create_redir_node(redir_type, file_tok->value, cmd));
// }

// static t_ast	*parse_redirections(t_parser *p)
// {
// 	t_ast	*cmd;

// 	cmd = parse_simple_cmd(p);
// 	if (!cmd)
// 		return (NULL);
// 	while (peek(p) && is_redirection(peek(p)->type))
// 	{
// 		cmd = parse_single_redir(p, cmd);
// 		if (!cmd)
// 			return (NULL);
// 	}
// 	return (cmd);
// }

// /* ************************************************************************** */
// /*                           SIMPLE COMMAND & PRIMARY                         */
// /* ************************************************************************** */

// static t_ast	*parse_simple_cmd(t_parser *p)
// {
// 	return (parse_primary(p));
// }

// /* Parse parentheses or command words */
// static t_ast	*parse_primary(t_parser *p)
// {
// 	t_ast	*node;

// 	if (match(p, T_OPEN_BRACKET))
// 	{
// 		advance(p);
// 		node = parse_or(p);
// 		if (!node)
// 			return (NULL);
// 		if (!expect(p, T_CLOSE_BRACKET))
// 		{
// 			free_ast(node);
// 			return (NULL);
// 		}
// 		return (node);
// 	}
// 	if (match(p, T_WORD))
// 		return (parse_word_sequence(p));
// 	return (NULL);
// }


// /* ************************************************************************** */
// /*                           MAIN PARSER ENTRY POINT                          */
// /* ************************************************************************** */

// t_ast	*parse(t_token *tokens)
// {
// 	t_parser	parser;
// 	t_ast	*ast;

// 	parser.tokens = tokens;
// 	parser.current = tokens;

// 	skip_spaces(&parser);

// 	ast = parse_or(&parser);
// 	if (!ast)
// 	{
// 		return (NULL);
// 	}

// 	printf("DEBUG: After parsing, current token:\n");
//     if (parser.current == NULL)
//         printf("  current = NULL (good!)\n");
//     else
//     {
//         printf("  current is NOT NULL (bad!)\n");
//         printf("  Type: %d\n", parser.current->type);
//         printf("  Value: '%s'\n", parser.current->value ? parser.current->value : "(null)");
//         if (parser.current->next)
//             printf("  Has next token\n");
//         else
//             printf("  No next token\n");
//     }

// 	skip_spaces(&parser);
// 	if (parser.current != NULL)
// 	{
// 		free_ast(ast);
// 		return (NULL);
// 	}
// 	return (ast);
// }

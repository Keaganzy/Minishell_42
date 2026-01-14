/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 14:53:35 by jotong            #+#    #+#             */
/*   Updated: 2026/01/14 21:37:27 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

t_shell	*init_shell(char **envp)
{
	t_shell	*shell;

	shell = malloc(sizeof(t_shell));
	if (!shell)
		return (NULL);
	shell->envp = dup_envp(shell, envp);
	if (!shell->envp)
	{
		free(shell);
		return (NULL);
	}
	shell->last_exit_status = 0;
	shell->exit_code = 0;
	shell->stdin_backup = -1;
	shell->stdout_backup = -1;
	shell->running = 1;
	shell->tokens = NULL;
	shell->ast = NULL;
	set_signals();
	return (shell);
}

static void	handle_sigint(t_shell *shell)
{
	if (g_sigint_received == 130 || g_sigint_received == 131)
	{
		shell->last_exit_status = g_sigint_received;
		g_sigint_received = 0;
	}
}

static int	process_line(char *line, t_shell *shell)
{
	t_token	*tokens;

	history_add(line);
	tokens = lex_input(line, shell);
	if (!tokens)
	{
		free(line);
		return (0);
	}
	shell->tokens = tokens;
	// print_token_stream_colored(tokens);
	shell->ast = parse(tokens, shell);
	// print_ast(shell->ast);
	if (shell->ast)
		execute_ast(shell->ast, shell);
	if (shell->ast)
		free_ast(shell->ast);
	token_free_all(&tokens);
	shell->tokens = NULL;
	shell->ast = NULL;
	free(line);
	return (1);
}

static int	shell_loop(t_shell *shell)
{
	char	*line;

	while (!shell->exit_code)
	{
		line = readline("MS$ ");
		handle_sigint(shell);
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		process_line(line, shell);
	}
	return (shell->last_exit_status);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;
	int		final_exit_status;

	(void)argc;
	(void)argv;
	g_sigint_received = 0;
	shell = init_shell(envp);
	if (!shell)
		return (1);
	final_exit_status = shell_loop(shell);
	rl_clear_history();
	cleanup_shell(shell);
	return (final_exit_status);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd_redirect_utils_new.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 16:56:13 by ksng              #+#    #+#             */
/*   Updated: 2026/01/07 21:44:17 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

int is_redirection(t_token_type type)
{
	return (type == T_REDIR_IN || type == T_REDIR_OUT
		|| type == T_REDIR_APPEND || type == T_HEREDOC);
}
t_node_type	get_redir_type(t_token_type type)
{
	if (type == T_REDIR_IN)
		return (N_REDIR_IN);
	if (type == T_REDIR_OUT)
		return (N_REDIR_OUT);
	if (type == T_REDIR_APPEND)
		return (N_REDIR_APPEND);
	if (type == T_HEREDOC)
		return (N_HEREDOC);
	return (N_CMD);
}

int	count_words(t_parser *p)
{
	int		count;
	t_token	*tmp;

	count = 0;
	tmp = p->current;
	while (tmp)
    {
        // Skip spaces
        while (tmp && tmp->type == T_SPACE)
            tmp = tmp->next;

        // Count word
        if (tmp && tmp->type == T_WORD)
        {
            count++;
            tmp = tmp->next;
        }
        else
            break;
    }
	return (count);
}

static void sigint_heredoc(int sig, siginfo_t *info, void *context)
{
    (void)context;
    (void)info;
    // g_sigint_received = 130;
    if (sig == SIGINT)
    {
        write(1, "\n", 1);
        // rl_on_new_line();
        // rl_replace_line("", 0);
        // rl_redisplay();
        g_sigint_received = 130;
        close(STDIN_FILENO);
    }
}

// char	*read_heredoc_content(char *delimiter)
// {
// 	char	*line;
// 	char	*content;
// 	char	*tmp;

// 	content = ft_strdup("");
// 	while (1)
// 	{
// 		line = readline("> ");
// 		if (!line || ft_strcmp(line, delimiter) == 0)
// 		{
// 			free(line);
// 			break ;
// 		}
// 		tmp = content;
// 		content = ft_strjoin(content, line);
// 		free(tmp);
// 		tmp = content;
// 		content = ft_strjoin(content, "\n");
// 		free(tmp);
// 		free(line);
// 	}
// 	return (content);
// }

char    *read_heredoc_content(char *delimiter)
{
    char    *line;
    char    *content;
    char    *tmp;
    struct sigaction    sa;
    ft_bzero(&sa, sizeof(sa));
    sa.sa_sigaction = sigint_heredoc;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO;
    // signal(SIGQUIT, SIG_IGN);
    if (sigaction(SIGINT, &sa, NULL) == -1)
    {
        perror("sigaction failed");
        exit(EXIT_FAILURE);
    }
    content = ft_strdup("");
    while (1)
    {
        line = readline("> ");
         if (g_sigint_received == 130)
        {
            free(line);
            free(content);
            exit(1);  /* Exit with code 1 */
        }
        if (!line || ft_strcmp(line, delimiter) == 0)
        {
            free(line);
            break ;
        }
        tmp = content;
        content = ft_strjoin(content, line);
        free(tmp);
        tmp = content;
        content = ft_strjoin(content, "\n");
        free(tmp);
        free(line);
    }
    // set_signals();
    return (content);
}

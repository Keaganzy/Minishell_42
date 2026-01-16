/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd_redirect_utils_1.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 18:04:40 by jotong            #+#    #+#             */
/*   Updated: 2026/01/16 18:04:41 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

void	handle_sigint_heredoc(char *line, char *content)
{
	free(line);
	free(content);
	exit(1);
}

int	check_heredoc_end(char *line, char *delimiter)
{
	if (!line || ft_strcmp(line, delimiter) == 0)
	{
		free(line);
		return (1);
	}
	return (0);
}

char	*append_line_to_content(char *content, char *line)
{
	char	*tmp;

	tmp = content;
	content = ft_strjoin(content, line);
	free(tmp);
	tmp = content;
	content = ft_strjoin(content, "\n");
	free(tmp);
	return (content);
}

char	*read_heredoc_content(char *delimiter)
{
	char	*line;
	char	*content;

	setup_sigact_heredoc(&content);
	while (1)
	{
		line = readline("> ");
		if (g_sigint_received == 130)
			handle_sigint_heredoc(line, content);
		if (check_heredoc_end(line, delimiter))
			break ;
		content = append_line_to_content(content, line);
		free(line);
	}
	return (content);
}

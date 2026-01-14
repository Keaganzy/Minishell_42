/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_for_heredoc.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 20:02:07 by jotong            #+#    #+#             */
/*   Updated: 2026/01/14 20:02:08 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

static char	*append_exit_status(char *expanded, t_shell *shell, int *i)
{
	char	*status_str;
	char	*new_str;

	status_str = ft_itoa(shell->last_exit_status);
	if (!status_str)
		return (expanded);
	new_str = ft_strjoin(expanded, status_str);
	*i += 2;
	free(expanded);
	free(status_str);
	return (new_str);
}

static char	*append_char(char *expanded, char c)
{
	char	buf[2];
	char	*new_str;

	buf[0] = c;
	buf[1] = '\0';
	new_str = ft_strjoin(expanded, buf);
	free(expanded);
	return (new_str);
}

static char	*append_env_var(char *expanded, char **line, int *i, t_shell *shell)
{
	char	*var_name;
	char	*var_value;
	char	*new_str;
	size_t	len;

	(*i)++;
	len = 0;
	while ((*line)[*i + len] && (ft_isalnum((*line)[*i + len])
		|| (*line)[*i + len] == '_'))
		len++;
	if (len == 0)
		return (append_char(expanded, '$'));
	var_name = ft_substr(*line, *i, len);
	var_value = getenv_value(shell->envp, var_name);
	if (var_value)
		new_str = ft_strjoin(expanded, var_value);
	else
		new_str = ft_strdup(expanded);
	free(expanded);
	*i += len;
	return (free(var_name), new_str);
}

char	*expand_heredoc_line(char *line, t_shell *shell)
{
	int		i;
	char	*expanded;

	i = 0;
	expanded = ft_strdup("");
	while (line[i])
	{
		if (line[i] == '$' && line[i + 1])
		{
			if (line[i + 1] == '?')
				expanded = append_exit_status(expanded, shell, &i);
			else
				expanded = append_env_var(expanded, &line, &i, shell);
		}
		else
		{
			expanded = append_char(expanded, line[i]);
			i++;
		}
	}
	free(line);
	return (expanded);
}

// char	*process_heredoc(char *delimiter, int is_quoted, t_shell *shell)
// {
// 	char	*line;
// 	char	*full_content;
// 	char	*tmp;

// 	full_content = ft_strdup("");
// 	while (1)
// 	{
// 		line = readline("> ");
// 		if (!line || ft_strcmp(line, delimiter) == 0)
// 		{
// 			free(line);
// 			break ;
// 		}
// 		// Only expand if the delimiter itself was not quoted 
// 		if (!is_quoted)
// 			line = expand_heredoc_line(line, shell);
// 		tmp = ft_strjoin(full_content, line);
// 		free(full_content);
// 		full_content = ft_strjoin(tmp, "\n");
// 		free(tmp);
// 		free(line);
// 	}
// 	return (full_content);
// }

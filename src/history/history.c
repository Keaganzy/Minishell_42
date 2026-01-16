/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 00:41:27 by jotong            #+#    #+#             */
/*   Updated: 2026/01/14 20:57:51 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

static	void	read_ms_history(const char *filename)
{
	int		fd;
	char	*line;
	int		i;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return ;
	line = get_next_line(fd);
	while (line != NULL)
	{
		i = 0;
		while (line[i] && line[i] != '\n')
			i++;
		if (line[i] == '\n')
			line[i] = '\0';
		add_history(line);
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
}

void	history_init(void)
{
	read_ms_history(HISTORY_FILE);
}

void	history_add(const char *line)
{
	if (*line)
		add_history(line);
}

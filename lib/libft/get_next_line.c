/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 09:05:09 by jotong            #+#    #+#             */
/*   Updated: 2025/07/17 23:19:07 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*rem_print(char *buf)
{
	char	*cleaned_str;
	size_t	i;
	size_t	len;

	i = 0;
	len = 0;
	while (buf[i] && buf[i] != '\n')
		i++;
	if (buf[i] == '\n')
		i++;
	len = ft_strlen(&buf[i]);
	cleaned_str = malloc(len + 1);
	if (cleaned_str)
	{
		cleaned_str[0] = '\0';
		ft_strlcpy(cleaned_str, &buf[i], len + 1);
	}
	free (buf);
	return (cleaned_str);
}

char	*get_line(char *buf)
{
	size_t	i;
	char	*to_print;

	i = 0;
	while (buf[i] && buf[i] != '\n')
		i++;
	if (buf[i] == '\n')
		i++;
	to_print = malloc(i + 1);
	if (!to_print)
		return (NULL);
	to_print[0] = '\0';
	ft_strlcpy(to_print, buf, i + 1);
	return (to_print);
}

char	*rd_parse_line(char **buf, int fd, char *tmp_buf, ssize_t *b)
{
	char	*out;

	if (!buf[fd])
	{
		buf[fd] = malloc(BUFFER_SIZE + 1);
		if (!buf[fd])
		{
			free(tmp_buf);
			return (NULL);
		}
		buf[fd][0] = '\0';
	}
	while (!ft_strchr(buf[fd], '\n'))
	{
		*b = read(fd, tmp_buf, BUFFER_SIZE);
		if (*b <= 0)
			break ;
		tmp_buf[*b] = '\0';
		out = ft_strjoin(buf[fd], tmp_buf);
		free(buf[fd]);
		buf[fd] = out;
	}
	free(tmp_buf);
	out = get_line(buf[fd]);
	return (out);
}

void	free_buf(char **buf, int fd)
{
	free(buf[fd]);
	buf[fd] = NULL;
}

char	*get_next_line(int fd)
{
	ssize_t		b;
	char		*tmp_buf;
	static char	*buf[1024];
	char		*out;
	char		*cleaned_tmp_str;

	b = 0;
	if (fd < 0 || fd > 1023 || BUFFER_SIZE <= 0)
		return (NULL);
	tmp_buf = malloc(BUFFER_SIZE + 1);
	if (!tmp_buf)
		return (NULL);
	tmp_buf[0] = '\0';
	out = rd_parse_line(buf, fd, tmp_buf, &b);
	cleaned_tmp_str = rem_print(buf[fd]);
	buf[fd] = cleaned_tmp_str;
	if (b <= 0 && buf[fd][0] == '\0')
		free_buf(buf, fd);
	if (out[0] == '\0')
	{
		free(out);
		out = NULL;
		return (NULL);
	}
	return (out);
}

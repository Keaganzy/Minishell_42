/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin-utils4.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 16:27:15 by jotong            #+#    #+#             */
/*   Updated: 2025/12/23 18:41:05 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

static int	find_chunk(char *str, char *chunk, int pos)
{
	size_t	len;
	int		i;

	len = ft_strlen(chunk);
	i = pos;
	while (str[i])
	{
		if (ft_strncmp(str + i, chunk, len) == 0)
			return (i + len);
		i++;
	}
	return (-1);
}

int	match_last(char *str, char **chunks, int n, int end_star)
{
	char	*last;
	size_t	l;
	size_t	sl;

	if (end_star || n == 0)
		return (0);
	last = chunks[n - 1];
	l = ft_strlen(last);
	sl = ft_strlen(str);
	if (sl < l)
		return (1);
	if (ft_strcmp(str + sl - l, last) != 0)
		return (1);
	return (0);
}

int	match_middle(char *str, char **chunks, int n, int *pos)
{
	int	i;
	int	newpos;

	i = 1;
	while (i < n - 1)
	{
		newpos = find_chunk(str, chunks[i], *pos);
		if (newpos < 0)
			return (1);
		*pos = newpos;
		i++;
	}
	return (0);
}

int	match_first(char *str, char **chunks, int *pos, int start_star)
{
	size_t	len;

	if (start_star || !chunks[0])
		return (0);
	len = ft_strlen(chunks[0]);
	if (ft_strncmp(str, chunks[0], len) != 0)
		return (1);
	*pos = len;
	return (0);
}

int	var_check(char *var)
{
	int	i;

	if (!var || !var[0])
		return (1);
	if (!(var[0] == '_' || ft_isalpha(var[0])))
		return (1);
	i = 0;
	while (var[i])
	{
		if (!ft_isalnum(var[i]) && var[i] != '_')
			return (1);
		i++;
	}
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strip_expand3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 13:17:26 by jotong            #+#    #+#             */
/*   Updated: 2026/01/16 13:26:08 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <dirent.h>
#include "libft.h"

int	match_pattern(char *str, char *pattern)
{
	if (ft_strcmp(str, ".") == 0 || ft_strcmp(str, "..") == 0)
		return (0);
	if (str[0] == '.' && pattern[0] != '.')
		return (0);
	return (match_recursive(str, pattern));
}

int	add_match(char **matches, int *count, char *name)
{
	if (*count >= 1023)
		return (0);
	matches[*count] = ft_strdup(name);
	if (!matches[*count])
		return (0);
	(*count)++;
	return (1);
}

int	add_dir_match(char **matches, int *count, char *dname, char *name)
{
	char	*full_fname;
	char	*tmp;

	if (*count >= 1023)
		return (0);
	tmp = ft_strjoin(dname, "/");
	if (!tmp)
		return (0);
	full_fname = ft_strjoin(tmp, name);
	free(tmp);
	if (!full_fname)
		return (0);
	matches[*count] = ft_strdup(full_fname);
	if (!matches[*count])
		return (0);
	(*count)++;
	return (1);
}

char	**get_matches(char *pattern)
{
	DIR				*dir;
	struct dirent	*entry;
	char			**matches;
	int				count;

	dir = opendir(".");
	if (!dir)
		return (NULL);
	matches = malloc(sizeof(char *) * 1024);
	if (!matches)
		return (closedir(dir), NULL);
	count = 0;
	entry = readdir(dir);
	while (entry)
	{
		if (ft_strcmp(entry->d_name, ".") != 0
			&& ft_strcmp(entry->d_name, "..") != 0
			&& !(entry->d_name[0] == '.' && pattern[0] != '.')
			&& match_pattern(entry->d_name, pattern))
			add_match(matches, &count, entry->d_name);
		entry = readdir(dir);
	}
	matches[count] = NULL;
	closedir(dir);
	if (count == 0)
		return (free(matches), NULL);
	return (matches);
}

char	*join_matches(char **matches)
{
	char	*result;
	char	*tmp;
	int		i;

	result = ft_strdup(matches[0]);
	if (!result)
		return (NULL);
	i = 1;
	while (matches[i])
	{
		tmp = result;
		result = ft_strjoin(result, " ");
		free(tmp);
		if (!result)
			return (NULL);
		tmp = result;
		result = ft_strjoin(result, matches[i]);
		free(tmp);
		if (!result)
			return (NULL);
		i++;
	}
	return (result);
}

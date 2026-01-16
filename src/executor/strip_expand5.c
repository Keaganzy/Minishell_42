#include "minishell.h"
#include <dirent.h>
#include "libft.h"

static int	should_add_match(struct dirent *entry, char *pattern)
{
	if (ft_strcmp(entry->d_name, ".") == 0)
		return (0);
	if (ft_strcmp(entry->d_name, "..") == 0)
		return (0);
	if (entry->d_name[0] == '.' && pattern[0] != '.')
		return (0);
	if (!match_pattern(entry->d_name, pattern))
		return (0);
	return (1);
}

char	**get_matches_in_dir(char *dname, char *pattern)
{
	DIR				*dir;
	struct dirent	*entry;
	char			**matches;
	int				count;

	dir = opendir(dname);
	if (!dir)
		return (NULL);
	matches = malloc(sizeof(char *) * 1024);
	if (!matches)
		return (closedir(dir), NULL);
	count = 0;
	entry = readdir(dir);
	while (entry)
	{
		if (should_add_match(entry, pattern))
			add_dir_match(matches, &count, dname, entry->d_name);
		entry = readdir(dir);
	}
	matches[count] = NULL;
	closedir(dir);
	if (count == 0)
		return (free(matches), NULL);
	return (matches);
}

void	free_matches(char **matches)
{
	int	i;

	if (!matches)
		return ;
	i = 0;
	while (matches[i])
		free(matches[i++]);
	free(matches);
}

int	has_wildcard(char *s, char *map, int start, int end)
{
	while (start < end)
	{
		if ((s[start] == '*' || s[start] == '?') && map[start] == 0)
			return (1);
		start++;
	}
	return (0);
}

int	word_end(char *s, int i)
{
	while (s[i] && !is_space(s[i]))
		i++;
	return (i);
}

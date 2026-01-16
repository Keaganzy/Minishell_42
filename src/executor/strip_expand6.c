#include "minishell.h"
#include "libft.h"

char	*expand_pattern(char *pattern)
{
	char	**matches;
	char	*result;
	char	*trimmed;
	char	*dir_name;

	trimmed = trim_pattern(pattern);
	if (!trimmed || !*trimmed)
		return (free(trimmed), ft_strdup(pattern));
	if (!ft_strnstr(pattern, "/", ft_strlen(pattern)))
		matches = get_matches(trimmed);
	else
	{
		dir_name = ft_strndup(pattern,
				ft_strnstr(pattern, "/", ft_strlen(pattern)) - pattern);
		matches = get_matches_in_dir(dir_name,
				trimmed + ft_strlen(dir_name) + 1);
	}
	free(trimmed);
	if (!matches)
		return (ft_strdup(pattern));
	result = join_matches(matches);
	free_matches(matches);
	if (!result)
		return (ft_strdup(pattern));
	return (result);
}

static size_t	calc_pattern_len(char *s, char *map, int start, int end)
{
	char	*pat;
	char	*exp;
	size_t	len;

	(void)map;
	pat = ft_substr(s, start, end - start);
	exp = expand_pattern(pat);
	len = ft_strlen(exp);
	free(pat);
	free(exp);
	return (len);
}

size_t	calc_wild_len(char *s, char *map)
{
	int		i;
	int		start;
	int		end;
	size_t	len;

	i = 0;
	len = 0;
	while (s[i])
	{
		if (is_space(s[i]))
			len++;
		else
		{
			start = i;
			end = word_end(s, i);
			if (has_wildcard(s, map, start, end))
				len += calc_pattern_len(s, map, start, end);
			else
				len += end - start;
			i = end;
			continue ;
		}
		i++;
	}
	return (len);
}

void	copy_word_wildcard(char *s, t_copy_word *cw, t_copy_state *cs)
{
	int	k;

	(void)s;
	cw->pat = ft_substr(s, cw->start, cw->end - cw->start);
	cw->exp = expand_pattern(cw->pat);
	k = 0;
	while (cw->exp[k])
		cs->result[cs->j++] = cw->exp[k++];
	free(cw->pat);
	free(cw->exp);
}

void	copy_word(char *s, char *map, int *i, t_copy_state *cs)
{
	t_copy_word	cw;

	cw.start = *i;
	cw.end = word_end(s, *i);
	if (has_wildcard(s, map, cw.start, cw.end))
		copy_word_wildcard(s, &cw, cs);
	else
	{
		while (*i < cw.end)
			cs->result[cs->j++] = s[(*i)++];
	}
	*i = cw.end;
}
//incase we need to update again.

// strip_expand2.c: Variable expansion helpers
// strip_expand4.c: Quote stripping and word splitting
// strip_expand5.c: Pattern matching utilities 
// strip_expand6.c: Wildcard expansion and main function 
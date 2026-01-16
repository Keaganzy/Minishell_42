#include "minishell.h"
#include "libft.h"

char	*trim_pattern(char *pattern)
{
	char	*start;
	char	*end;
	size_t	len;

	start = pattern;
	while (*start && is_space(*start))
		start++;
	if (!*start)
		return (ft_strdup(""));
	end = start + ft_strlen(start) - 1;
	while (end > start && is_space(*end))
		end--;
	len = end - start + 1;
	return (ft_substr(start, 0, len));
}

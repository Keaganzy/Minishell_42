#include "minishell.h"
#include "libft.h"

char	*split_words_exec(char *s, char *map)
{
	char	*result;
	size_t	len;

	len = calc_split_len(s, map);
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	do_split_words(s, map, result);
	return (result);
}

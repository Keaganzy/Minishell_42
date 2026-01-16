/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strip_expand9.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 18:05:35 by jotong            #+#    #+#             */
/*   Updated: 2026/01/16 18:05:35 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

char	*expand_wild(char *s, char *map)
{
	t_copy_state	cs;
	size_t			len;
	int				i;

	len = calc_wild_len(s, map);
	cs.result = malloc(len + 1);
	if (!cs.result)
		return (NULL);
	i = 0;
	cs.j = 0;
	while (s[i])
	{
		if (is_space(s[i]))
			cs.result[cs.j++] = s[i++];
		else
			copy_word(s, map, &i, &cs);
	}
	cs.result[cs.j] = '\0';
	return (cs.result);
}

char	*expand_and_replace(char **s, t_shell *shell)
{
	char	*step1;
	char	*map;
	char	*step2;
	char	*step3;

	if (!s || !*s)
		return (NULL);
	step1 = expand_strip(*s, shell, &map);
	if (!step1)
		return (NULL);
	step2 = split_words_exec(step1, map);
	free(step1);
	if (!step2)
		return (free(map), NULL);
	step3 = expand_wild(step2, map);
	free(step2);
	free(map);
	if (!step3)
		return (NULL);
	free(*s);
	*s = step3;
	return (step3);
}

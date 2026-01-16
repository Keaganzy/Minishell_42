/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strip_expand7.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 18:05:29 by jotong            #+#    #+#             */
/*   Updated: 2026/01/16 18:05:30 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

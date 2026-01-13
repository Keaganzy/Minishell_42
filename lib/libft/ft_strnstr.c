/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 21:22:38 by jotong            #+#    #+#             */
/*   Updated: 2025/07/15 12:29:09 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*find_substr(const char *str, const char *to_find, size_t len)
{
	char		*hay;
	char		*needle;
	size_t		pos;
	size_t		str_len;

	pos = 0;
	while (*str && pos <= len)
	{
		str_len = 0;
		hay = (char *)str;
		needle = (char *)to_find;
		while (*hay && *needle && (*hay == *needle) && (pos + str_len) < len)
		{
			hay++;
			needle++;
			str_len++;
		}
		if (*needle == '\0')
			return ((char *)str);
		str++;
		pos++;
	}
	return (NULL);
}

char	*ft_strnstr(const char *str, const char *to_find, size_t len)
{
	char	*result;

	result = NULL;
	if (*str == '\0' && *to_find == '\0')
		return ((char *)str);
	else if (*str == '\0')
		return (NULL);
	result = find_substr(str, to_find, len);
	return (result);
}

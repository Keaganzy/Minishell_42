/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 09:31:21 by jotong            #+#    #+#             */
/*   Updated: 2025/05/23 15:21:06 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	len_create(char const *s, unsigned int start, size_t len)
{
	unsigned int		real_len;
	unsigned int		actual_len;

	real_len = ft_strlen(s);
	if (start >= real_len)
		return (0);
	actual_len = real_len - start;
	if (actual_len > len)
		actual_len = len;
	return (actual_len);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char				*substr;
	unsigned int		real_len;
	unsigned int		str_len;

	real_len = ft_strlen(s);
	str_len = len_create(s, start, len);
	if (!s || len == 0 || real_len < start + 1)
		return (ft_strdup(""));
	substr = (char *)malloc(str_len + 1);
	if (!substr)
		return (NULL);
	else
	{
		ft_strlcpy(substr, s + start, str_len + 1);
		return (substr);
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 14:28:27 by jotong            #+#    #+#             */
/*   Updated: 2025/07/17 23:18:13 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char			*new_str;
	unsigned int	total_len;

	total_len = 0;
	if (!s1 && !s2)
		return (NULL);
	if (s1 != NULL)
		total_len += ft_strlen(s1);
	if (s2 != NULL)
		total_len += ft_strlen(s2);
	new_str = (char *)malloc(total_len + 1);
	if (!new_str)
		return (NULL);
	new_str[0] = '\0';
	if (s1)
		ft_strlcat(new_str, s1, total_len + 1);
	if (s2)
		ft_strlcat(new_str, s2, total_len + 1);
	return (new_str);
}

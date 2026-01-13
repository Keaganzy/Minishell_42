/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:44:42 by jotong            #+#    #+#             */
/*   Updated: 2025/05/23 15:13:09 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char	*to;
	unsigned char	*from;
	size_t			len;

	len = 0;
	to = (unsigned char *)dest;
	from = (unsigned char *)src;
	if (!to || !from)
		return (dest);
	else
	{
		while (len < n)
		{
			to[len] = from[len];
			len++;
		}
	}
	return (dest);
}

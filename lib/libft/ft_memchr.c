/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 19:40:08 by jotong            #+#    #+#             */
/*   Updated: 2025/09/28 21:01:11 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	const unsigned char		*ptr;
	unsigned char			target;
	size_t					count;

	count = 0;
	target = (unsigned char)c;
	ptr = (const unsigned char *)s;
	while (count < n)
	{
		if (*ptr == target)
			return ((void *)ptr);
		else
		{
			count++;
			ptr++;
		}
	}
	return (NULL);
}

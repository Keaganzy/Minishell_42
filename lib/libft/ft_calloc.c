/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 22:39:43 by jotong            #+#    #+#             */
/*   Updated: 2025/05/23 15:10:58 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t num, size_t size)
{
	unsigned char		*arr;

	arr = NULL;
	if (num <= 0 || size <= 0)
		return (malloc(0));
	if (num == (size_t)INT_MAX || size == (size_t)INT_MAX)
		return (NULL);
	if ((num > 0 && size > SIZE_MAX / num) || (num * size) / num != size)
		return (NULL);
	arr = malloc(num * size);
	if (!arr)
		return (NULL);
	ft_memset(arr, 0, (num * size));
	return (arr);
}

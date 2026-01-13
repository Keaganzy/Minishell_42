/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 23:27:29 by jotong            #+#    #+#             */
/*   Updated: 2025/05/23 15:15:48 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *str, int c)
{
	char	*ptr;
	char	*latest;

	ptr = (char *)str;
	latest = NULL;
	while (*ptr)
	{
		if (*ptr == (unsigned char)c)
			latest = ptr;
		ptr++;
	}
	if (*ptr == (char )c)
		return ((char *)ptr);
	else
		return ((char *)latest);
}

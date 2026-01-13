/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 08:17:47 by jotong            #+#    #+#             */
/*   Updated: 2025/05/23 15:15:31 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	int		count;
	int		len;
	char	*s1;

	count = 0;
	if (!s || !f)
		return (NULL);
	len = ft_strlen(s);
	s1 = malloc(len + 1);
	while (count < len)
	{
		s1[count] = f(count, s[count]);
		count++;
	}
	s1[len] = '\0';
	return (s1);
}

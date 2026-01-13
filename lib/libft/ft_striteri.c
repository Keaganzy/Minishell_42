/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 22:02:37 by jotong            #+#    #+#             */
/*   Updated: 2025/05/13 22:34:07 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char *))
{
	int	count;
	int	len;

	count = 0;
	if (!(!s || !f))
	{
		len = ft_strlen(s);
		while (count < len)
		{
			f(count, s);
			s++;
			count++;
		}
	}
}

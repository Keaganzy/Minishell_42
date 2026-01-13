/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_p_ptr.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 21:36:59 by jotong            #+#    #+#             */
/*   Updated: 2025/07/09 17:40:05 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_p_ptr(void *s, const char *hex)
{
	int				count;
	unsigned long	addr;
	char			buffer[20];
	int				i;

	i = 19;
	count = 0;
	addr = (unsigned long)s;
	if (s == 0)
		return (write(1, "(nil)", 5));
	while (addr && i >= 2)
	{
		buffer[i--] = hex[addr % 16];
		addr /= 16;
	}
	buffer[i] = 'x';
	i--;
	buffer[i] = '0';
	count += write(1, &buffer[i], 20 - i);
	return (count);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_p_nbr_base.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 13:57:13 by jotong            #+#    #+#             */
/*   Updated: 2025/07/09 17:39:34 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_p_nbr_base(unsigned int nbr, char *base)
{
	int		i;
	char	buffer[32];
	int		count;

	count = 0;
	i = 0;
	if (nbr == 0)
	{
		count += write(1, "0", 1);
		return (count);
	}
	while (nbr > 0)
	{
		buffer[i] = base[nbr % 16];
		nbr /= 16;
		i++;
	}
	while (i-- > 0)
		count += write(1, &buffer[i], 1);
	return (count);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_p_unbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 13:47:53 by jotong            #+#    #+#             */
/*   Updated: 2025/07/09 17:39:53 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_p_unbr(unsigned int n, char type)
{
	int		count;
	char	string;

	count = 0;
	if (n == 0)
	{
		count += write(1, "0", 1);
		return (count);
	}
	if (n < 10)
	{
		string = n + '0';
		count += write(1, &string, 1);
		return (count);
	}
	count += ft_p_nbr(n / 10, type);
	string = (n % 10) + '0';
	count += write(1, &string, 1);
	return (count);
}

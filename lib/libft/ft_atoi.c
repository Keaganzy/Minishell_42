/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 13:51:16 by jotong            #+#    #+#             */
/*   Updated: 2025/05/20 17:37:04 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_atoi(const char *str)
{
	unsigned int	final_int;
	int				sign;
	unsigned char	*ptr;

	ptr = (unsigned char *)str;
	final_int = 0;
	sign = 1;
	while (*ptr == '\t' || *ptr == '\n' || *ptr == '\v'
		|| *ptr == '\f' || *ptr == '\r' || *ptr == ' ')
		ptr++;
	if ((*ptr == '-' || *ptr == '+'))
	{
		if (*ptr == '-')
			sign *= -1 ;
		ptr++;
	}
	while (*ptr >= '0' && *ptr <= '9')
	{
		final_int = final_int * 10 + *ptr - '0';
		ptr++;
	}
	return (final_int * sign);
}

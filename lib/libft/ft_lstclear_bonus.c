/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 17:14:37 by jotong            #+#    #+#             */
/*   Updated: 2025/06/17 22:32:34 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*curr;
	t_list	*succ;

	curr = *lst;
	while (curr -> next)
	{
		succ = curr -> next;
		ft_lstdelone(curr, del);
		curr = succ;
	}
	ft_lstdelone(curr, del);
	*lst = NULL;
}

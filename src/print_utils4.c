/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils4.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 15:04:09 by jotong            #+#    #+#             */
/*   Updated: 2026/01/02 15:20:44 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

void	print_token_stream(t_token *tokens)
{
	t_token	*curr;
	int		count;

	curr = tokens;
	count = 0;
	printf("\nToken Stream: ");
	while (curr)
	{
		printf("[");
		if (curr->value && curr->value[0])
			printf("%s", curr->value);
		else
			printf("%s", token_type_to_string(curr->type));
		printf("]");
		if (curr->next)
			printf(" → ");
		curr = curr->next;
		count++;
		if (count > 100)
		{
			printf(" [ERROR: Too many tokens or circular list]");
			break ;
		}
	}
	printf("\n\n");
}

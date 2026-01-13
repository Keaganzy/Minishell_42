/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 00:41:27 by jotong            #+#    #+#             */
/*   Updated: 2026/01/02 15:42:59 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	history_init(void)
{
	read_history(HISTORY_FILE);
}

void	history_add(const char *line)
{
	if (*line)
		add_history(line);
}

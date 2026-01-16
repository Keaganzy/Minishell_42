/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin-utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 10:18:40 by jotong            #+#    #+#             */
/*   Updated: 2026/01/16 10:18:41 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

static int	handle_tilde(char *chunk, size_t *k,
		char **s_final, t_shell *shell)
{
	char	*val;

	if (*k == 0 && chunk[*k] == '~'
		&& (chunk[*k + 1] == '\0' || chunk[*k + 1] == '/'))
	{
		val = getenv_value(shell->envp, "HOME");
		if (val)
			*s_final = ft_strjoin_and_free(s_final, val, 1);
		(*k)++;
		if (chunk[*k] == '/')
			(*k)++;
		return (1);
	}
	return (0);
}

static int	handle_dollar(char *chunk, size_t *k,
		char **s_final, t_shell *shell)
{
	if (chunk[*k] == '$')
	{
		*k = expand_variable(chunk, *k, s_final, shell);
		return (1);
	}
	return (0);
}

char	*handle_dollars_tilde(char *chunk, t_shell *shell)
{
	size_t	k;
	char	*s_final;

	k = 0;
	s_final = NULL;
	while (chunk[k])
	{
		if (handle_tilde(chunk, &k, &s_final, shell))
			continue ;
		if (handle_dollar(chunk, &k, &s_final, shell))
			continue ;
		s_final = ft_strjoin_char_and_free(&s_final, chunk[k], 1);
		k++;
	}
	if (!s_final)
		return (ft_strdup(""));
	return (s_final);
}

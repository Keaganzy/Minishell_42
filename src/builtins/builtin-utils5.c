/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin-utils5.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 15:57:28 by jotong            #+#    #+#             */
/*   Updated: 2026/01/02 15:58:13 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

static size_t	expand_exit_status(char **s_final, t_shell *shell, size_t k)
{
	char	*val;

	val = ft_itoa(shell->last_exit_status);
	*s_final = ft_strjoin_and_free(s_final, val, 1);
	return (k + 1);
}

static int	get_var_name_len(char *chunk, size_t k)
{
	int	v_len;

	v_len = 0;
	if (ft_isalpha(chunk[k]) || chunk[k] == '_')
	{
		v_len++;
		while (ft_isalnum(chunk[k + v_len]) || chunk[k + v_len] == '_')
			v_len++;
	}
	return (v_len);
}

static size_t	expand_named_var(char *chunk, size_t k,
		char **s_final, t_shell *shell)
{
	char	*val;
	char	*v_name;
	int		v_len;

	v_len = get_var_name_len(chunk, k);
	if (v_len > 0)
	{
		v_name = ft_strndup(&chunk[k], v_len);
		val = getenv_value(shell->envp, v_name);
		if (val)
			*s_final = ft_strjoin_and_free(s_final, val, 1);
		free(v_name);
		return (k + v_len);
	}
	*s_final = ft_strjoin_and_free(s_final, "$", 1);
	return (k);
}

size_t	expand_variable(char *chunk, size_t k,
		char **s_final, t_shell *shell)
{
	k++;
	if (chunk[k] == '?')
		return (expand_exit_status(s_final, shell, k));
	return (expand_named_var(chunk, k, s_final, shell));
}

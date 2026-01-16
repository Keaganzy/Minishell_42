/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strip_expand.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 13:14:15 by jotong            #+#    #+#             */
/*   Updated: 2026/01/16 13:15:14 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <dirent.h>
#include "libft.h"

void	init_quote_state(t_quote_state *state)
{
	state->in_single = 0;
	state->in_double = 0;
	state->in_curly_brackets = 0;
	state->flag = 0;
}

size_t	get_var_len(char *s)
{
	size_t	len;

	len = 0;
	if (s[len] == '?' || (s[len] >= '0' && s[len] <= '9'))
		return (1);
	while (s[len] && (ft_isalnum(s[len]) || s[len] == '_'))
		len++;
	return (len);
}

char	*get_env_value(char *var_name, t_shell *shell)
{
	int		i;
	size_t	len;

	if (!var_name || !shell->envp)
		return (NULL);
	len = ft_strlen(var_name);
	if (len == 1 && *var_name == '?')
		return (ft_itoa(shell->last_exit_status));
	if (len == 1 && *var_name == '0')
		return (ft_strdup("minishell"));
	i = 0;
	while (shell->envp[i])
	{
		if (ft_strncmp(shell->envp[i], var_name, len) == 0
			&& shell->envp[i][len] == '=')
			return (shell->envp[i] + len + 1);
		i++;
	}
	return (NULL);
}

char	*get_home_dir(t_shell *shell)
{
	char	*home;

	home = get_env_value("HOME", shell);
	if (!home)
		return ("/tmp");
	return (home);
}

int	match_recursive(char *str, char *pat)
{
	if (!*pat)
		return (!*str);
	if (*pat == '*')
	{
		while (*pat == '*')
			pat++;
		if (!*pat)
			return (1);
		while (*str)
		{
			if (match_recursive(str, pat))
				return (1);
			str++;
		}
		return (0);
	}
	if (*str && (*str == *pat || *pat == '?'))
		return (match_recursive(str + 1, pat + 1));
	return (0);
}

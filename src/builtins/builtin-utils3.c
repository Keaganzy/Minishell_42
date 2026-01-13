/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin-utils3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 16:34:29 by jotong            #+#    #+#             */
/*   Updated: 2026/01/11 07:05:39 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

static char	*create_single_char_string(char c)
{
	char	*new_s;

	new_s = malloc(2 * sizeof(char));
	if (!new_s)
		return (NULL);
	ft_memcpy(new_s, &c, 1);
	new_s[1] = '\0';
	return (new_s);
}

static char	*append_char_to_string(char *old_s, char c, int to_free)
{
	char	*new_s;
	size_t	s_len;

	s_len = ft_strlen(old_s);
	new_s = malloc((s_len + 2) * sizeof(char));
	if (!new_s)
		return (NULL);
	ft_memcpy(new_s, old_s, ft_strlen(old_s));
	new_s[s_len] = c;
	new_s[s_len + 1] = '\0';
	if (to_free == 1 && old_s)
		free(old_s);
	return (new_s);
}

char	*ft_strjoin_char_and_free(char **new_s, char c, int to_free)
{
	char	*result;

	if (!(*new_s) && !c)
		return (NULL);
	if (!c)
		return (*new_s);
	if (!(*new_s))
	{
		*new_s = create_single_char_string(c);
		return (*new_s);
	}
	result = append_char_to_string(*new_s, c, to_free);
	if (!result)
		return (NULL);
	*new_s = result;
	return (*new_s);
}

int	add_update_env_vars(t_shell *shell, const char *av)
{
	char	*key;
	int		eq_pos;
	int		status;

	eq_pos = find_equals_pos(av);
	key = ft_strndup(av, eq_pos);
	status = validate_key(key);
	if (status == 0)
		status = set_env_variable(shell, av, eq_pos, key);
	free(key);
	return (status);
}

void	free_substr(char **substr)
{
	int	j;

	j = 0;
	while (substr[j])
	{
		free(substr[j]);
		substr[j] = NULL;
		j++;
	}
	free(substr);
	substr = NULL;
}

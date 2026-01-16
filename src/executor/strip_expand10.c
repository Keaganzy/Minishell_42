/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strip_expand10.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 18:05:37 by jotong            #+#    #+#             */
/*   Updated: 2026/01/16 18:05:38 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

int	exp_var_single(char **s, t_exp *e, size_t len)
{
	size_t	j;

	e->out[e->i] = '$';
	e->map[e->i++] = 1;
	j = 0;
	while (j < len)
	{
		e->out[e->i] = (*s)[j];
		e->map[e->i++] = 1;
		j++;
	}
	*s += len;
	return (1);
}

static void	exp_var_copy_value(char *value, t_exp *e)
{
	size_t	j;

	j = 0;
	if (value && *value)
	{
		while (value[j])
		{
			e->out[e->i] = value[j++];
			e->map[e->i++] = e->state.in_double;
		}
	}
}

int	exp_var(char **s, t_exp *e, t_shell *shell)
{
	char	*name;
	char	*value;
	size_t	len;

	(*s)++;
	len = get_var_len(*s);
	if (len == 0)
	{
		e->out[e->i] = '$';
		e->map[e->i++] = (e->state.in_single || e->state.in_double);
		return (1);
	}
	if (e->state.in_single)
		return (exp_var_single(s, e, len));
	name = ft_substr(*s, 0, len);
	if (!name)
		return (0);
	value = get_env_value(name, shell);
	exp_var_copy_value(value, e);
	if (len == 1 && (*name == '?' || *name == '0'))
		free(value);
	free(name);
	*s += len;
	return (1);
}

char	*join_matches(char **matches)
{
	char	*result;
	char	*tmp;
	int		i;

	result = ft_strdup(matches[0]);
	if (!result)
		return (NULL);
	i = 1;
	while (matches[i])
	{
		tmp = result;
		result = ft_strjoin(result, " ");
		free(tmp);
		if (!result)
			return (NULL);
		tmp = result;
		result = ft_strjoin(result, matches[i]);
		free(tmp);
		if (!result)
			return (NULL);
		i++;
	}
	return (result);
}

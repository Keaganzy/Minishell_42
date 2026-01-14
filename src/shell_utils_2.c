/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utils_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 17:44:57 by jotong            #+#    #+#             */
/*   Updated: 2026/01/14 21:03:40 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

void	print_err(const char *s, char *av)
{
	write(STDERR_FILENO, s, ft_strlen(s));
	write(STDERR_FILENO, ": ", 2);
	write(STDERR_FILENO, strerror(errno), ft_strlen(strerror(errno)));
	if (av)
	{
		write(STDERR_FILENO, ": ", 2);
		write(STDERR_FILENO, av, ft_strlen(av));
	}
	write(1, "\n", 1);
}

char	**dup_envp(t_shell *shell, char **envp)
{
	char	**duped;
	int		i;
	int		count;

	i = 0;
	count = 0;
	while (envp[count])
		count++;
	duped = malloc(sizeof(char *) * (count + 1));
	if (!duped)
		return (NULL);
	while (i < count)
	{
		duped[i] = ft_strdup(envp[i]);
		if (!duped[i])
			return (cleanup_dup_envp(shell, i));
		i++;
	}
	duped[i] = NULL;
	return (duped);
}

char	**realloc_ms(char **original, size_t new_size)
{
	char	**tmp;
	size_t	i;

	tmp = original;
	original = malloc(sizeof(char *) * (new_size + 1));
	if (!original)
		return (NULL);
	i = 0;
	while (tmp[i] && i < new_size)
	{
		original[i] = tmp[i];
		i++;
	}
	original[i] = NULL;
	free(tmp);
	return (original);
}

int	unsetenv_value(char ***envp, const char *key)
{
	int		i;
	int		j;
	size_t	k_len;

	i = 0;
	j = 0;
	k_len = ft_strlen(key);
	while ((*envp)[i])
	{
		if (ft_strncmp((*envp)[i], key, k_len) == 0
			&& (*envp)[i][k_len] == '=')
			free((*envp)[i]);
		else
		{
			(*envp)[j] = (*envp)[i];
			j++;
		}
		i++;
	}
	(*envp)[j] = NULL;
	*envp = realloc_ms(*envp, j);
	return (0);
}

char	*getenv_value(char **env, const char *key)
{
	int		i;
	size_t	len;

	if (!env || !key)
		return (NULL);
	len = ft_strlen(key);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], key, len) == 0 && env[i][len] == '=')
			return (env[i] + len + 1);
		i++;
	}
	return (NULL);
}

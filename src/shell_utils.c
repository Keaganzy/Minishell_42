/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 00:15:14 by jotong            #+#    #+#             */
/*   Updated: 2026/01/11 09:28:47 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

static char	*create_env_string(const char *key, const char *value)
{
	size_t	key_len;
	char	*new_var;

	key_len = ft_strlen(key);
	new_var = malloc(key_len + ft_strlen(value) + 2);
	if (!new_var)
		return (NULL);
	ft_strlcpy(new_var, key, key_len + ft_strlen(value) + 2);
	new_var[key_len] = '=';
	ft_strlcpy(new_var + key_len + 1, value, ft_strlen(value) + 1);
	return (new_var);
}

static int	update_existing_var(char **envp, const char *key, char *new_var)
{
	size_t	key_len;
	int		i;

	key_len = ft_strlen(key);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], key, key_len) == 0
			&& envp[i][key_len] == '=')
		{
			free(envp[i]);
			envp[i] = new_var;
			return (1);
		}
		i++;
	}
	return (0);
}

static char	**create_new_env(char **old_env, char *new_var, int size)
{
	char	**new_env;
	int		j;

	new_env = malloc(sizeof(char *) * (size + 2));
	if (!new_env)
	{
		free(new_var);
		return (NULL);
	}
	j = 0;
	while (j < size)
	{
		new_env[j] = old_env[j];
		j++;
	}
	new_env[size] = new_var;
	new_env[size + 1] = NULL;
	return (new_env);
}

int	setenv_value(char ***envp, const char *key, const char *value)
{
	char	*new_var;
	char	**new_env;
	int		i;

	new_var = create_env_string(key, value);
	if (!new_var)
		return (1);
	if (update_existing_var(*envp, key, new_var))
		return (0);
	i = 0;
	while ((*envp)[i])
		i++;
	new_env = create_new_env(*envp, new_var, i);
	if (!new_env)
		return (1);
	free(*envp);
	*envp = new_env;
	return (0);
}

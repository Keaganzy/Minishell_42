/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env4.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 09:09:46 by jotong            #+#    #+#             */
/*   Updated: 2026/01/11 14:38:47 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

void	sort_envp_vars(char **env_var)
{
	int		i;
	int		j;
	int		count;
	char	*temp;

	i = 0;
	count = 0;
	while (env_var[count])
		count++;
	while (i < count - i - 1)
	{
		j = 0;
		while (j < count - i - 1)
		{
			if (ft_strcmp(env_var[j], env_var[j + 1]) > 0)
			{
				temp = env_var[j];
				env_var[j] = env_var[j + 1];
				env_var[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
}

void	print_env_vars_env(char **duped_env)
{
	int		i;

	i = 0;
	while (duped_env[i])
	{
		if (ft_strchr(duped_env[i], '=') == NULL)
			continue ;
		else
			printf("%s\n", duped_env[i]);
		i++;
	}
}

void	print_env_vars_export(char **duped_env)
{
	int		i;
	int		eq_pos;
	char	*key;

	i = 0;
	while (duped_env[i])
	{
		if (ft_strchr(duped_env[i], '=') == NULL)
			printf("declare -x %s\n", duped_env[i]);
		else
		{
			eq_pos = find_equals_pos(duped_env[i]);
			key = ft_strndup(duped_env[i], eq_pos);
			printf("declare -x %s=\"%s\"\n", key, duped_env[i] + eq_pos + 1);
			free(key);
		}
		i++;
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 21:44:16 by jotong            #+#    #+#             */
/*   Updated: 2026/01/11 12:31:09 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#ifdef __linux__
# include <limits.h>
#endif

static void	extract_and_update_vars(char **av, t_shell *shell, int *status)
{
	int	i;
	int	result;

	i = 1;
	result = 0;
	while (av[i])
	{
		if (ft_strchr(av[i], '=') != NULL)
		{
			if (add_update_env_vars(shell, av[i]) != 0)
				*status = 1;
		}
		else
		{
			result = validate_key(av[i]);
			if (result != 0)
				*status = 1;
		}
		i++;
	}
	return ;
}

int	ft_export(char **av, t_shell *shell)
{
	int		status;
	char	**duped_env;

	status = 0;
	if (!shell || !shell->envp)
		return (1);
	if (av[1])
	{
		extract_and_update_vars(av, shell, &status);
		return (status);
	}
	else
	{
		duped_env = dup_envp(shell, shell->envp);
		if (!duped_env)
			return (1);
		sort_envp_vars(duped_env);
		print_env_vars_export(duped_env);
		free_array(duped_env);
	}
	return (status);
}

int	ft_env(char **av, t_shell *shell)
{
	char	**duped_env;

	if (av[1])
	{
		ft_putstr_fd("env: too many arguments\n", 2);
		return (1);
	}
	if (!shell || !shell->envp)
		return (1);
	duped_env = dup_envp(shell, shell->envp);
	if (!duped_env)
		return (1);
	sort_envp_vars(duped_env);
	print_env_vars_env(duped_env);
	free_array(duped_env);
	return (0);
}

int	validate_identifiers(char **av)
{
	int	i;
	int	j;

	i = 1;
	j = 0;
	while (av[i])
	{
		j = 1;
		if (!(ft_isalpha(av[i][0]) || av[i][0] == '-'))
			return (0);
		while (av[i][j])
		{
			if (!ft_isalnum(av[i][j]) || av[i][j] == '-')
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}

int	ft_pwd(char **av, t_shell *shell)
{
	char	c[PATH_MAX];

	(void)av;
	(void)shell;
	if (getcwd(c, sizeof(c)) != 0)
	{
		printf("%s\n", c);
		return (0);
	}
	else
	{
		perror("pwd");
		return (1);
	}
}

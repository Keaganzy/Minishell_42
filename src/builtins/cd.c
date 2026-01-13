/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 22:22:11 by jotong            #+#    #+#             */
/*   Updated: 2026/01/11 14:35:00 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

static void	handle_path(char *av, t_shell *shell, char **path)
{
	if (!av || ft_strcmp(av, "~") == 0)
	{
		*path = getenv_value(shell->envp, "HOME");
	}
	else if (ft_strcmp(av, "-") == 0)
	{
		*path = getenv_value(shell->envp, "OLDPWD");
		if (*path)
			printf("%s\n", *path);
	}
	else
	{
		*path = av;
	}
}

static int	update_pwd_vars(t_shell *shell, char *prev_pwd)
{
	char	cwd[4096];

	if (!getcwd(cwd, sizeof(cwd)))
	{
		printf("cd: error retrieving directory\n");
		if (prev_pwd)
			setenv_value(&(shell->envp), "OLDPWD", prev_pwd);
		setenv_value(&(shell->envp), "PWD", cwd);
		return (0);
	}
	if (prev_pwd)
		setenv_value(&(shell->envp), "OLDPWD", prev_pwd);
	setenv_value(&(shell->envp), "PWD", cwd);
	return (0);
}

int	builtin_cd(char **av, t_shell *shell)
{
	char	*path;
	char	*prev_pwd;

	handle_path(av[1], shell, &path);
	if (!path)
	{
		printf("cd: HOME not set\n");
		return (1);
	}
	prev_pwd = getenv_value(shell->envp, "PWD");
	if (chdir(path) != 0)
	{
		if (av[2])
			ft_putstr_fd("cd: too many arguments\n", 2);
		else
			print_err("cd", path);
		shell->last_exit_status = chdir(path);
		return (1);
	}
	return (update_pwd_vars(shell, prev_pwd));
}

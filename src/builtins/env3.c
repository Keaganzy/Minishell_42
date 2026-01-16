/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 16:51:27 by codespace         #+#    #+#             */
/*   Updated: 2026/01/16 10:19:09 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#ifdef __linux__
# include <limits.h>
#endif

int	ft_unset(char **av, t_shell *shell)
{
	int	i;

	i = 1;
	if (!av)
		return (1);
	while (av[i])
	{
		if (validate_identifiers(av) != 1)
			return (0);
		else if (getenv_value(shell->envp, av[i]) != NULL)
			unsetenv_value(&shell->envp, av[i]);
		i++;
	}
	return (0);
}

void	handle_exit_and_cleanup(int exit_code, t_shell *shell)
{
	shell->exit_code = exit_code;
	cleanup_shell(shell);
	exit(exit_code);
}

int	has_overflow(char *str)
{
	long long	result;
	int			sign;
	int			i;

	i = 0;
	sign = 1;
	result = 0;
	if (str[i] == '+' || str[i] == '-')
		if (str[i++] == '-')
			sign = -1;
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (sign == 1 && result > (LLONG_MAX - (str[i] - '0')) / 10)
			return (1);
		if (sign == -1 && result > (-(LLONG_MIN + (str[i] - '0')) / 10))
			return (1);
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (0);
}

void	print_numeric_error(char *arg, t_shell *shell)
{
	ft_putstr_fd("exit: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(": numeric argument required\n", 2);
	handle_exit_and_cleanup(2, shell);
}

int	check_numeric_arg(char *arg)
{
	int	i;

	i = 0;
	if (arg[i] == '+' || arg[i] == '-')
		i++;
	if (!arg[i])
		return (0);
	while (arg[i])
	{
		if (arg[i] < '0' || arg[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

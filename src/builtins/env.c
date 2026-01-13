/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 22:22:11 by jotong            #+#    #+#             */
/*   Updated: 2026/01/11 14:29:20 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#ifdef __linux__
# include <limits.h>
#endif

static int	count_args(char **av)
{
	int	i;

	i = 1;
	while (av[i] && av[i][0] != '\0')
		i++;
	return (i);
}

static void	handle_exit_and_cleanup(int exit_code, t_shell *shell)
{
	shell->exit_code = exit_code;
	cleanup_shell(shell);
	exit(exit_code);
}

static void	print_numeric_error(char *arg, t_shell *shell)
{
	ft_putstr_fd("exit: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(": numeric argument required\n", 2);
	handle_exit_and_cleanup(2, shell);
}

static int	handle_exit_dashes(char **av, int num_args, t_shell *shell,
	int *exit_code)
{
	if (ft_strcmp(av[1], "--") == 0)
	{
		if (num_args == 2)
		{
			*exit_code = shell->last_exit_status;
			return (1);
		}
		else if (num_args > 3)
		{
			*exit_code = 1;
			return (ft_putstr_fd("exit: too many arguments\n", 2), 1);
		}
		else if (!check_numeric_arg(av[2]))
			return (print_numeric_error(av[2], shell), 1);
		else if (has_overflow(av[2]))
			return (print_numeric_error(av[2], shell), 1);
		*exit_code = (unsigned char)ft_atoi(av[2]);
		return (1);
	}
	return (0);
}

int	ft_exit(char **av, t_shell *shell)
{
	int	num_args;
	int	exit_code;

	num_args = count_args(av);
	ft_putstr_fd("exit\n", 1);
	exit_code = shell->last_exit_status;
	if (num_args >= 2)
	{
		if (handle_exit_dashes(av, num_args, shell, &exit_code))
			return (handle_exit_and_cleanup(exit_code, shell), 0);
		if (!check_numeric_arg(av[1]))
			print_numeric_error(av[1], shell);
		if (num_args > 2)
			return (ft_putstr_fd("exit: too many arguments\n", 2), 1);
		if (has_overflow(av[1]))
			print_numeric_error(av[1], shell);
		else
			exit_code = (unsigned char)ft_atoi(av[1]);
	}
	return (handle_exit_and_cleanup(exit_code, shell), 0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 22:22:11 by jotong            #+#    #+#             */
/*   Updated: 2026/01/11 13:26:25 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

static int	is_flag_n(char *arg)
{
	int	i;

	if (arg[0] != '-')
		return (0);
	i = 1;
	while (arg[i] == 'n')
		i++;
	return (arg[i] == '\0');
}

static void	handle_empty_output(char **av, int i, char **output)
{
	if (!av[i])
		*output = ft_strdup("");
	else
		*output = ft_strdup(av[i]);
	return ;
}

static char	*manipulate_echo_arg(char **av, int i, char *output)
{
	char	*tmp;

	while (av[i])
	{
		if (!output)
			handle_empty_output(av, i, &output);
		else
		{
			if (av[i][0])
			{
				tmp = output;
				output = ft_strjoin(output, " ");
				free(tmp);
			}
			tmp = output;
			output = ft_strjoin(output, av[i]);
			free(tmp);
		}
		i++;
	}
	return (output);
}

static char	*add_newline(char *output)
{
	char	*tmp;

	tmp = output;
	output = ft_strjoin(output, "\n");
	free(tmp);
	return (output);
}

int	ft_echo(char **av, t_shell *shell)
{
	int		i;
	int		n_flag;
	char	*output;

	(void)shell;
	i = 1;
	n_flag = 0;
	output = NULL;
	while (av[i] && av[i][0] == '-' && is_flag_n(av[i]))
	{
		n_flag = 1;
		i++;
	}
	output = manipulate_echo_arg(av, i, output);
	if (!output)
		output = ft_strdup("");
	if (!n_flag)
		output = add_newline(output);
	write(1, output, ft_strlen(output));
	return (free(output), 0);
}

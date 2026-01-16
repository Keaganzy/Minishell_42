/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strip_expand2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 16:38:28 by jotong            #+#    #+#             */
/*   Updated: 2026/01/16 13:26:00 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

size_t	calc_var_len_helper(char *s, t_shell *shell, int *need_free)
{
	size_t	len;
	char	*name;
	char	*value;

	len = get_var_len(s);
	if (len == 0)
		return (1);
	name = ft_substr(s, 0, len);
	if (!name)
		return (0);
	value = get_env_value(name, shell);
	*need_free = (len == 1 && (name[0] == '?' || name[0] == '0'));
	free(name);
	if (!value)
		return (0);
	len = ft_strlen(value);
	if (*need_free)
		free(value);
	return (len);
}

static int	handle_tilde_var(char **s, size_t *len, t_quote_state *state,
				t_calc_len_params *p)
{
	int	need_free;

	if (**s == '~' && !state->in_single && !state->in_double
		&& (*s == p->start || *(*s - 1) == ' '))
	{
		*len += ft_strlen(get_home_dir(p->shell));
		(*s)++;
		return (1);
	}
	if (**s == '$' && !state->in_single && *(*s + 1))
	{
		(*s)++;
		*len += calc_var_len_helper(*s, p->shell, &need_free);
		*s += get_var_len(*s);
		return (1);
	}
	return (0);
}

void	process_calc_len_char(char **s, size_t *len, t_quote_state *state,
			t_calc_len_params *p)
{
	if (**s == '\'' && !state->in_double)
	{
		state->in_single = !state->in_single;
		(*s)++;
	}
	else if (**s == '"' && !state->in_single)
	{
		state->in_double = !state->in_double;
		(*s)++;
	}
	else if (handle_tilde_var(s, len, state, p))
		return ;
	else
	{
		(*len)++;
		(*s)++;
	}
}

size_t	calc_len(char *s, t_shell *shell)
{
	size_t				len;
	t_quote_state		state;
	t_calc_len_params	params;

	params.start = s;
	params.shell = shell;
	len = 0;
	init_quote_state(&state);
	while (*s)
		process_calc_len_char(&s, &len, &state, &params);
	return (len);
}

int	exp_tilde(char **s, t_exp *e, t_shell *shell)
{
	char	*home;
	int		is_quoted;
	size_t	j;

	home = get_home_dir(shell);
	is_quoted = (e->state.in_single || e->state.in_double);
	j = 0;
	while (home[j])
	{
		e->out[e->i] = home[j++];
		e->map[e->i++] = is_quoted;
	}
	(*s)++;
	return (1);
}

//continue on 10;
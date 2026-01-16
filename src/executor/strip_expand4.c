#include "minishell.h"
#include "libft.h"

int	expand_process_char(char **s, t_exp *e, t_shell *shell)
{
	if (**s == '\'' && !e->state.in_double)
		return (e->state.in_single = !e->state.in_single, (*s)++, 1);
	else if (**s == '"' && !e->state.in_single)
		return (e->state.in_double = !e->state.in_double, (*s)++, 1);
	else if (**s == '~' && !e->state.in_single && !e->state.in_double)
		return (exp_tilde(s, e, shell));
	else if (**s == '$' && !e->state.in_single && *(*s + 1))
		return (exp_var(s, e, shell));
	else
		return (e->out[e->i] = **s, e->map[e->i++] = (e->state.in_single
				|| e->state.in_double), (*s)++, 1);
}

char	*expand_strip(char *s, t_shell *shell, char **map_out)
{
	t_exp	e;
	size_t	len;

	len = calc_len(s, shell);
	e.out = malloc(len + 1);
	e.map = malloc(len + 1);
	if (!e.out || !e.map)
		return (free(e.out), free(e.map), NULL);
	e.i = 0;
	init_quote_state(&e.state);
	while (*s)
		if (!expand_process_char(&s, &e, shell))
			return (free(e.out), free(e.map), NULL);
	e.out[e.i] = '\0';
	e.map[e.i] = '\0';
	*map_out = e.map;
	return (e.out);
}

static void	update_split_len(char c, char m, size_t *len, int *prev)
{
	if (is_space(c) && m == 0)
	{
		if (!*prev)
		{
			(*len)++;
			*prev = 1;
		}
	}
	else
	{
		(*len)++;
		*prev = 0;
	}
}

size_t	calc_split_len(char *s, char *map)
{
	size_t	len;
	int		i;
	int		prev_was_space;

	len = 0;
	prev_was_space = 1;
	i = 0;
	while (s[i])
	{
		update_split_len(s[i], map[i], &len, &prev_was_space);
		i++;
	}
	if (prev_was_space && len > 0)
		len--;
	return (len);
}

void	do_split_words(char *s, char *map, char *result)
{
	int		i;
	int		j;
	int		prev_was_sep;

	i = 0;
	j = 0;
	prev_was_sep = 1;
	while (s[i])
	{
		if (is_space(s[i]) && map[i] == 0)
		{
			if (!prev_was_sep && j > 0)
			{
				result[j++] = ' ';
				prev_was_sep = 1;
			}
		}
		else
		{
			result[j++] = s[i];
			prev_was_sep = 0;
		}
		i++;
	}
	result[j] = '\0';
}

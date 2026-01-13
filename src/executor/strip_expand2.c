/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strip_expand2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotong <jotong@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 16:38:28 by jotong            #+#    #+#             */
/*   Updated: 2026/01/01 21:17:49 by jotong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <dirent.h>
#include "libft.h"

/* ************************************************************************** */
/*                           UTILITY FUNCTIONS                                */
/* ************************************************************************** */

static void init_quote_state(t_quote_state *state)
{
	state->in_single = 0;
	state->in_double = 0;
	state->in_curly_brackets = 0;
	state->flag = 0;
}

static size_t get_var_len(char *s)
{
	size_t len;

	len = 0;
	if (s[len] == '?' || (s[len] >= '0' && s[len] <= '9'))
		return (1);
	// else if (s[len] == '0')
	// 	return (10);
	while (s[len] && (ft_isalnum(s[len]) || s[len] == '_'))
		len++;
	return (len);
}

static char *get_env_value(char *var_name, t_shell *shell)
{
	int i;
	size_t len;

	if (!var_name || !shell->envp)
		return (NULL);
	len = ft_strlen(var_name);
	if (len == 1 && *var_name == '?')
		return (ft_itoa(shell->last_exit_status));
	if (len == 1 && *var_name == '0')
		return (ft_strdup("minishell"));
	i = 0;
	while (shell->envp[i])
	{
		if (ft_strncmp(shell->envp[i], var_name, len) == 0 && shell->envp[i][len] == '=')
			return (shell->envp[i] + len + 1);
		i++;
	}
	return (NULL);
}

static char *get_home_dir(t_shell *shell)
{
	char *home;

	home = get_env_value("HOME", shell);
	if (!home)
		return ("/tmp");
	return (home);
}

/* ************************************************************************** */
/*                           WILDCARD MATCHING                                */
/* ************************************************************************** */

static int match_recursive(char *str, char *pat)
{
	if (!*pat)
		return (!*str);
	if (*pat == '*')
	{
		while (*pat == '*')
			pat++;
		if (!*pat)
			return (1);
		while (*str)
		{
			if (match_recursive(str, pat))
				return (1);
			str++;
		}
		return (0);
	}
	if (*str && (*str == *pat || *pat == '?'))
		return (match_recursive(str + 1, pat + 1));
	return (0);
}

int match_pattern(char *str, char *pattern)
{
	if (ft_strcmp(str, ".") == 0 || ft_strcmp(str, "..") == 0)
		return (0);
	if (str[0] == '.' && pattern[0] != '.')
		return (0);
	return (match_recursive(str, pattern));
}

static int add_match(char **matches, int *count, char *name)
{
	if (*count >= 1023)
		return (0);
	matches[*count] = ft_strdup(name);
	if (!matches[*count])
		return (0);
	(*count)++;
	return (1);
}

static int add_dir_match(char **matches, int *count, char *dname, char *name)
{
	char *full_fname;
	char *tmp;

	if (*count >= 1023)
		return (0);
	tmp = ft_strjoin(dname, "/");
	if (!tmp)
		return (0);
	full_fname = ft_strjoin(tmp, name);
	free(tmp);
	if (!full_fname)
		return (0);
	matches[*count] = ft_strdup(full_fname);
	if (!matches[*count])
		return (0);
	(*count)++;
	return (1);
}

static char **get_matches(char *pattern)
{
	DIR *dir;
	struct dirent *entry;
	char **matches;
	int count;

	dir = opendir(".");
	if (!dir)
		return (NULL);
	matches = malloc(sizeof(char *) * 1024);
	if (!matches)
		return (closedir(dir), NULL);
	count = 0;
	entry = readdir(dir);
	while (entry)
	{
		if (ft_strcmp(entry->d_name, ".") != 0 && ft_strcmp(entry->d_name, "..") != 0 && !(entry->d_name[0] == '.' && pattern[0] != '.') && match_pattern(entry->d_name, pattern))
			add_match(matches, &count, entry->d_name);
		entry = readdir(dir);
	}
	matches[count] = NULL;
	closedir(dir);
	if (count == 0)
		return (free(matches), NULL);
	return (matches);
}

static char **get_matches_in_dir(char *dname, char *pattern)
{
	DIR *dir;
	struct dirent *entry;
	char **matches;
	int count;

	dir = opendir(dname);
	if (!dir)
		return (NULL);
	matches = malloc(sizeof(char *) * 1024);
	if (!matches)
		return (closedir(dir), NULL);
	count = 0;
	entry = readdir(dir);
	while (entry)
	{
		if (ft_strcmp(entry->d_name, ".") != 0 && ft_strcmp(entry->d_name, "..") != 0 && !(entry->d_name[0] == '.' && pattern[0] != '.') && match_pattern(entry->d_name, pattern))
			add_dir_match(matches, &count, dname, entry->d_name);
		entry = readdir(dir);
	}
	matches[count] = NULL;
	closedir(dir);
	if (count == 0)
		return (free(matches), NULL);
	return (matches);
}

static void free_matches(char **matches)
{
	int i;

	if (!matches)
		return;
	i = 0;
	while (matches[i])
		free(matches[i++]);
	free(matches);
}

static char *join_matches(char **matches)
{
	char *result;
	char *tmp;
	int i;

	result = ft_strdup(matches[0]);
	if (!result)
		return (NULL);
	i = 1;
	while (matches[i])
	{
		tmp = result;
		result = ft_strjoin(result, " ");
		free(tmp);
		if (!result)
			return (NULL);
		tmp = result;
		result = ft_strjoin(result, matches[i]);
		free(tmp);
		if (!result)
			return (NULL);
		i++;
	}
	return (result);
}

/* ************************************************************************** */
/*                      STEP 1: LENGTH CALCULATION                            */
/* ************************************************************************** */

static size_t calc_var_len_helper(char *s, t_shell *shell, int *need_free)
{
	size_t len;
	char *name;
	char *value;

	len = get_var_len(s);
	if (len == 0)
		return (1);
	name = ft_substr(s, 0, len);
	if (!name)
		return (0);
	value = get_env_value(name, shell);
	if (len == 1 && (name[0] == '?' || name[0] == '0'))
		*need_free = 1;
	else
		*need_free = 0;
	free(name);
	if (!value)
		return (0);
	len = ft_strlen(value);
	if (*need_free)
		free(value);
	return (len);
}

static size_t calc_len(char *s, t_shell *shell)
{
	size_t len;
	t_quote_state state;
	char *start;
	int need_free;

	start = s;
	len = 0;
	init_quote_state(&state);
	while (*s)
	{
		if (*s == '\'' && !state.in_double)
		{
			state.in_single = !state.in_single;
			s++;
		}
		else if (*s == '"' && !state.in_single)
		{
			state.in_double = !state.in_double;
			s++;
		}
		else if (*s == '~' && !state.in_single && !state.in_double && (s == start || *(s - 1) == ' '))
		{
			len += ft_strlen(get_home_dir(shell));
			s++;
		}
		else if (*s == '$' && !state.in_single && *(s + 1))
		{
			s++;
			len += calc_var_len_helper(s, shell, &need_free);
			s += get_var_len(s);
		}
		else
		{
			len++;
			s++;
		}
	}
	return (len);
}

/* ************************************************************************** */
/*                      STEP 2: EXPAND AND STRIP QUOTES                       */
/* ************************************************************************** */

static int exp_tilde(char **s, t_exp *e, t_shell *shell)
{
	char *home;
	int is_quoted;
	size_t j;

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

static int exp_var(char **s, t_exp *e, t_shell *shell)
{
	char *name;
	char *value;
	size_t len;
	size_t j;

	(*s)++;
	len = get_var_len(*s);
	if (len == 0)
		return (e->out[e->i] = '$', e->map[e->i++] = (e->state.in_single
				|| e->state.in_double), 1);

	// CRITICAL: Don't expand if in single quotes
	if (e->state.in_single)
	{
		// Just copy the $varname literally
		e->out[e->i] = '$';
		e->map[e->i++] = 1;  // Mark as quoted
		j = 0;
		while (j < len)
		{
			e->out[e->i] = (*s)[j];
			e->map[e->i++] = 1;
			j++;
		}
		*s += len;
		return (1);
	}
	name = ft_substr(*s, 0, len);
	if (!name)
		return (0);
	value = get_env_value(name, shell);
	j = 0;
	if (value && *value)
	{
		while (value[j])
		{
			e->out[e->i] = value[j++];
			e->map[e->i++] = e->state.in_double;
		}
	}

	if (len == 1 && (*name == '?' || *name == '0'))
		free(value);
	free(name);
	*s += len;
	return (1);
}

static int process_char(char **s, t_exp *e, t_shell *shell)
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
		return (e->out[e->i] = **s, e->map[e->i++] = (e->state.in_single || e->state.in_double), (*s)++, 1);
}

static char *expand_strip(char *s, t_shell *shell, char **map_out)
{
	t_exp e;
	size_t len;

	len = calc_len(s, shell);
	e.out = malloc(len + 1);
	e.map = malloc(len + 1);
	if (!e.out || !e.map)
		return (free(e.out), free(e.map), NULL);
	e.i = 0;
	init_quote_state(&e.state);
	while (*s)
		if (!process_char(&s, &e, shell))
			return (free(e.out), free(e.map), NULL);
	e.out[e.i] = '\0';
	e.map[e.i] = '\0';
	*map_out = e.map;
	return (e.out);
}

/* ************************************************************************** */
/*                      STEP 2.5: WORD SPLITTING                              */
/* ************************************************************************** */

static size_t calc_split_len(char *s, char *map)
{
	size_t len;
	int i;
	int prev_was_space;

	len = 0;
	prev_was_space = 1;
	i = 0;
	while (s[i])
	{
		// Only treat as separator if UNQUOTED space (map[i] == 0)
		if (is_space(s[i]) && map[i] == 0)
		{
			if (!prev_was_space)
			{
				len++;
				prev_was_space = 1;
			}
		}
		else
		{
			len++;
			prev_was_space = 0;
		}
		i++;
	}
	if (prev_was_space && len > 0)
		len--;
	return (len);
}

static char *split_words(char *s, char *map)
{
	char *result;
	size_t len;
	int i;
	int j;
	int prev_was_sep;

	len = calc_split_len(s, map);
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	prev_was_sep = 1;
	while (s[i])
	{
		// Only treat as separator if UNQUOTED space (map[i] == 0)

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
	return (result);
}

/* ************************************************************************** */
/*                      STEP 3: WILDCARD EXPANSION                            */
/* ************************************************************************** */

static int has_wildcard(char *s, char *map, int start, int end)
{
	while (start < end)
	{
		if ((s[start] == '*' || s[start] == '?') && map[start] == 0)
			return (1);
		start++;
	}
	return (0);
}

static int word_end(char *s, int i)
{
	while (s[i] && !is_space(s[i]))
		i++;
	return (i);
}

static char *trim_pattern(char *pattern)
{
	char *start;
	char *end;
	size_t len;

	start = pattern;
	while (*start && is_space(*start))
		start++;
	if (!*start)
		return (ft_strdup(""));
	end = start + ft_strlen(start) - 1;
	while (end > start && is_space(*end))
		end--;
	len = end - start + 1;
	return (ft_substr(start, 0, len));
}

static char *expand_pattern(char *pattern)
{
	char **matches;
	char *result;
	char *trimmed;
	char *dir_name;

	trimmed = trim_pattern(pattern);
	if (!trimmed || !*trimmed)
		return (free(trimmed), ft_strdup(pattern));
	if (!ft_strnstr(pattern, "/", ft_strlen(pattern)))
	{
		matches = get_matches(trimmed);
	}
	else
	{
		dir_name = ft_strndup(pattern, ft_strnstr(pattern, "/", ft_strlen(pattern)) - pattern);
		matches = get_matches_in_dir(dir_name, trimmed + ft_strlen(dir_name) + 1);
	}
	free(trimmed);
	if (!matches)
		return (ft_strdup(pattern));
	result = join_matches(matches);
	free_matches(matches);
	if (!result)
		return (ft_strdup(pattern));
	return (result);
}

static size_t calc_wild_len(char *s, char *map)
{
	int i;
	int start;
	int end;
	char *pat;
	char *exp;
	size_t len;

	i = 0;
	len = 0;
	while (s[i])
	{
		if (is_space(s[i]))
			len++, i++;
		else
		{
			start = i;
			end = word_end(s, i);
			if (has_wildcard(s, map, start, end))
			{
				pat = ft_substr(s, start, end - start);
				exp = expand_pattern(pat);
				len += ft_strlen(exp);
				free(pat);
				free(exp);
			}
			else
				len += end - start;
			i = end;
		}
	}
	return (len);
}

static void copy_word(char *s, char *map, int *i, char **result, int *j)
{
	int start;
	int end;
	char *pat;
	char *exp;
	int k;

	start = *i;
	end = word_end(s, *i);
	if (has_wildcard(s, map, start, end))
	{
		pat = ft_substr(s, start, end - start);
		exp = expand_pattern(pat);
		k = 0;
		while (exp[k])
			(*result)[(*j)++] = exp[k++];
		free(pat);
		free(exp);
	}
	else
	{
		while (*i < end)
			(*result)[(*j)++] = s[(*i)++];
	}
	*i = end;
}

static char *expand_wild(char *s, char *map)
{
	char *result;
	size_t len;
	int i;
	int j;

	len = calc_wild_len(s, map);
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (s[i])
	{
		if (is_space(s[i]))
			result[j++] = s[i++];
		else
			copy_word(s, map, &i, &result, &j);
	}
	result[j] = '\0';
	return (result);
}

/* ************************************************************************** */
/*                           MAIN ENTRY POINT                                 */
/* ************************************************************************** */

char *expand_and_replace(char **s, t_shell *shell)
{
	char *step1;
	char *map;
	char *step2;
	char *step3;

	if (!s || !*s)
		return (NULL);
	// printf("s: '%s'\n", *s);
	step1 = expand_strip(*s, shell, &map);
	// printf("step 1: '%s'\n", step1);
	if (!step1)
		return (NULL);
	step2 = split_words(step1, map);
	// printf("step 2: '%s'\n", step2);
	free(step1);
	if (!step2)
		return (free(map), NULL);
	step3 = expand_wild(step2, map);
	free(step2);
	free(map);
	if (!step3)
		return (NULL);
	free(*s);
	*s = step3;
	return (step3);
}

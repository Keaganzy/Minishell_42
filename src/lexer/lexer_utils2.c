/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   lexer_utils2.c									 :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: jotong <jotong@student.42.fr>			  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2025/12/03 14:16:27 by jotong			#+#	#+#			 */
/*   Updated: 2025/12/21 21:32:59 by jotong		   ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

char	*extract_word_with_inv_commas(const char *s, size_t *i)
{
	size_t			start;
	unsigned char	inv_comma;

	inv_comma = s[*i];
	start = *i;
	(*i)++;
	while (s[*i] && s[*i] != inv_comma)
		(*i)++;
	(*i)++;
	return (ft_strndup(s + start, *i - start));
}

t_token_type	handle_space(const char *s, size_t *i)
{
	(*i)++;
	while (is_space((unsigned char)s[*i]))
		(*i)++;
	return (T_SPACE);
}

int	is_fd_redir(const char *s, size_t i)
{
	if (!ft_isdigit(s[i]))
		return (0);
	while (ft_isdigit(s[i]))
		i++;
	if (s[i] == '<' || s[i] == '>')
		return (1);
	return (0);
}

int	extract_fd_for_lexer(const char *s, size_t *i)
{
	size_t	start;
	int		fd;

	start = *i;
	while (ft_isdigit(s[*i]))
		(*i)++;
	fd = ft_atoi(s + start);
	return (fd);
}

char	*ft_strjoin_and_free(char **new_s, char *substr, int to_free)
{
	char	*tmp;

	if (!(*new_s) && !substr)
		return (NULL);
	if (!substr)
		return (*new_s);
	if (!(*new_s))
	{
		*new_s = substr;
		return (*new_s);
	}
	tmp = *new_s;
	*new_s = ft_strjoin(*new_s, substr);
	if ((to_free == 1 || to_free == 3) && tmp)
		free(tmp);
	if ((to_free == 2 || to_free == 3) && substr)
		free(substr);
	return (*new_s);
}

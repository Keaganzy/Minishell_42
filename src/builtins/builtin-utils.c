/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   builtin-utils.c									:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: jotong <jotong@student.42singapore.sg>	 +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2025/10/21 23:07:01 by jotong			#+#	#+#			 */
/*   Updated: 2025/12/23 16:14:56 by jotong		   ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include <dirent.h>
#include "minishell.h"
#include "libft.h"

static int	has_outer_quotes(char *str, size_t len)
{
	if (len >= 2 && ((str[0] == '"' && str[len - 1] == '"')
			|| (str[0] == '\'' && str[len - 1] == '\'')))
		return (1);
	return (0);
}

static char	*strip_outer_quotes(char *str)
{
	size_t	len;
	char	*result;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	if (has_outer_quotes(str, len))
	{
		result = ft_substr(str, 1, len - 2);
		return (result);
	}
	return (ft_strdup(str));
}

int	find_equals_pos(const char *av)
{
	int	i;

	i = 0;
	while (av[i] && av[i] != '=')
		i++;
	return (i);
}

int	validate_key(char *key)
{
	if (var_check(key) != 0)
	{
		printf("export: `%s': not a valid identifier\n", key);
		return (1);
	}
	return (0);
}

int	set_env_variable(t_shell *shell, const char *av,
		int eq_pos, char *key)
{
	char	*val;
	char	*val_stripped;
	int		status;

	val = ft_strdup(&av[eq_pos + 1]);
	val_stripped = strip_outer_quotes(val);
	free(val);
	status = 0;
	if (setenv_value(&shell->envp, key, val_stripped) != 0)
		status = 1;
	free(val_stripped);
	return (status);
}

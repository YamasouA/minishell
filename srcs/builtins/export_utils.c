#include "minishell.h"

bool	is_valid_var(char *str) // shareable unset func
{
	int	i;

	if (!str)
		return (false);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (false);
	i = 1;
	while (str[i])
	{
		if ((!ft_isalnum(str[i]) && str[i] != '_'))
			return (false);
		i++;
	}
	return (true);
}

int	is_append_flag(char **key, char *eq_pos)
{
	char	*tmp;

	if (eq_pos && eq_pos != *key && *(eq_pos - 1) == '+' && *key[0] != '+')
	{
		*(eq_pos - 1) = '\0'; //move below if (is_append_flag) ?
		tmp = *key;
		*key = ft_strjoin(*key, eq_pos);
		free(tmp);
		if (*key == NULL)
			err_exit("malloc error: ");
		return (1);
	}
	return (0);
}

void	free_args(char **args) //same free_strs
{
	size_t	i;

	i = 0;
	while (args[i])
	{
		free(args[i++]);
	}
	free(args);
}

#include "minishell.h"

int	which_update_flag(char **key)
{
	char	*var_name;
	char	*eq_pos;
	int		flag;

	flag = EXPORT_NONE;
	eq_pos = ft_strchr(*key, '=');
	if (is_append_flag(key, eq_pos))
	{
		flag = EXPORT_APPEND;
		eq_pos = ft_strchr(*key, '=');
		var_name = ft_substr(*key, 0, eq_pos - key[0]);
	}
	else
	{
		flag = EXPORT_NEW;
		var_name = ft_substr(*key, 0, eq_pos - key[0]);
	}
	if (!is_valid_var(var_name))
	{
		free(var_name);
		print_invalid_identifier(*key);
		return (EXPORT_ERROR);
	}
	free(var_name);
	return (flag);
}

bool	is_valid_var(char *str)
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
		*(eq_pos - 1) = '\0';
		tmp = *key;
		*key = ft_strjoin(*key, eq_pos);
		free(tmp);
		if (*key == NULL)
			err_exit("malloc error: ");
		return (1);
	}
	return (0);
}

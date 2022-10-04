#include "minishell.h"

bool	is_valid_unset_args(char *str)
{
	int	i;
	
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

void	ft_unset(char **strs, char *envp[])
{
	int	i;
	int	j;
	char	**split_env;

	i = 1;
	while (strs[i])
	{
		if (!is_valid_unset_args(strs[i]))
		{
			ft_putstr_fd("minishell: unset: `", 2);
			ft_putstr_fd(strs[i++], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			continue ;
		}
		j = 0;
		while (envp[j])
		{
			split_env = ft_split(envp[j], '=');
			if (ft_strlen(strs[i]) == ft_strlen(split_env[0])
				&& ft_strncmp(strs[i], split_env[0], ft_strlen(strs[i] + 1)) == 0)
			{
				while (envp[j])
				{
					envp[j] = envp[j + 1];
					j++;
				}
				free(split_env);
				break ;
			}
			free(split_env);
			j++;
		}
		i++;
	}
}

int	main(int argc, char **argv, char *envp[])
{
	int	i;

	ft_unset(argv, envp);
	i = 0;
	printf("\n==env_list==\n%s\n", envp[i++]);
	while (envp[i])
	{
		printf("%s\n", envp[i++]);
	}
	return (0);
}

#include "minishell.h"

char	*find_env(char *var, size_t len)
{
	t_env	*tmp;

	tmp = g_sh_var.environ;
	while (tmp)
	{
		if (ft_strlen(tmp->key) == len
			&& ft_strncmp(tmp->key, var, len) == 0)
		{
			if (tmp->value != NULL)
				return (ft_strdup(tmp->value));
		}
		tmp = tmp->next;
	}
	return (ft_strdup(""));
}

t_env	*search_env(t_env *env, char *key)
{
	while (env)
	{
		if (ft_strncmp(env->key, key, ft_strlen(key) + 1) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

char	*search_key(t_env *env, char *key)
{
	while (env)
	{
//		printf("key:%s\n", env->key);
		if (ft_strncmp(env->key, key, ft_strlen(key) + 1) == 0)
			return (env->value);
		env = env->next;
	}
//	printf("key:%s\n", env->key);
	return (NULL);
}
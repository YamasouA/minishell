#include "minishell.h"

char	**envlist_to_str(t_env *env)
{
	t_env	*head;
	size_t	cnt;
	size_t	i;
	char	**envstr;

	cnt = count_envlist(env);
	envstr = (char **)malloc(sizeof(char *) * (cnt + 1));
	if (envstr == NULL)
		err_exit("malloc error: ");
	i = -1;
	head = env;
	while (head != NULL)
	{
		if (head->value)
			envstr[++i] = join_with_connector(head->key, head->value, '=');
		if (envstr[i] == NULL)
			err_exit("malloc error: ");
		head = head->next;
	}
	envstr[++i] = NULL;
	return (envstr);
}

void	update_or_add_value(t_env **env, char *key, char *value)
{
	t_env	*tmp;
	t_env	*target;
	t_env	*last;

	tmp = *env;
	if (*env == NULL)
	{
		*env = env_new(key, value);
		return ;
	}
	target = search_env(*env, key);
	if (target != NULL)
	{
		free(target->value);
		target->value = ft_xstrdup(value);
	}
	else
	{
		last = env_last(*env);
		last->next = env_new(key, value);
	}
	*env = tmp;
}

t_env	*create_env(void)
{
	extern char	**environ;
	t_env		*env;
	t_env		*new;
	int			i;

	i = 0;
	env = NULL;
	while (environ[i])
	{
		new = (t_env *)malloc(sizeof(t_env) * 1);
		if (new == NULL)
			err_exit("malloc error: ");
		else
			set_data(environ[i], new);
		add_env(&env, new);
		i++;
	}
	return (env);
}
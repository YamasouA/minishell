#include "minishell.h"

t_env	*env_last(t_env *env)
{
	int	i;

	i = 0;
	if (env == NULL)
		return (NULL);
	while (env->next != NULL)
		env = env->next;
	return (env);
}
void	add_env(t_env **env, t_env *new)
{
	t_env	*last;

	if (*env == NULL)
	{
		*env = new;
		return ;
	}
	last = env_last(*env);
	last->next = new;
}


void	set_data(char *environ, t_env *new)
{
	char	key;
	char	value;
	char	*eq_pos;

	if (new == NULL)
		printf("error");
	eq_pos = ft_strchr(environ, '=');
	if (eq_pos == NULL)
	{
		new->key = ft_strdup(environ);
		if (new->key == NULL)
			printf("error");
		new->value = NULL;
	}
	else
	{
		new->key = ft_substr(environ, 0, eq_pos - environ);
		new->value = ft_strdup(eq_pos + 1);
		if (new->key == NULL || new->value == NULL)
			printf("error");
	}
	new->is_env = true;
	new->next = NULL;
}

t_env	*search_env(t_env *env, char *key)
{
	while (env)
	{
		if (ft_strncmp(env->key, key, ft_strlen(key)+1) == 0)
			return env;
		env = env->next;
	}
	return (NULL);
}

char	*search_key(t_env *env, char *key)
{
	while (env)
	{
		if (ft_strncmp(env->key, key, ft_strlen(key)+1) == 0)
			return env->value;
		env = env->next;
	}
	return (NULL);
}


t_env	*create_env()
{
	extern char	**environ;
	t_env	*env;
	t_env	*new;
	int	i;

	i = 0;
	env = NULL;
	while (environ[i])
	{
		new = (t_env *)malloc(sizeof(t_env) * 1);
		if (new == NULL)
			return (NULL);
		else
			set_data(environ[i], new);
		add_env(&env, new);
		i++;
	}
	return (env);
}

void	print_env(t_env *env)
{
	while (env)
	{
		printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
}

int main()
{
	t_env	*env;
	env = create_env();
	print_env(env);
	char	*value;
	value = search_key(env, "jfaowij");
	printf("value: %s\n", value);
	value = search_key(env, "_");
	printf("value: %s\n", value);
	value = search_key(env, "HOME");
	printf("value: %s\n", value);
}

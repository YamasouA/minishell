#include "minishell.h"

void	print_prefix_env(t_env *envp)
{
	while (envp)
	{
		if (envp->value)
			printf("declare -x %s=\"%s\"\n", envp->key, envp->value);
		else
			printf("declare -x %s\n", envp->key);
		envp = envp->next;
	}
}

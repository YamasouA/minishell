#include "minishell.h"

static char	*create_and_check_path(char *env_path, char *cmd_name)
{
	char	*join_path;

	join_path = join_with_connector(env_path, cmd_name, '/');
	if (join_path == NULL)
		err_exit("malloc error: ");
	if (!access(join_path, X_OK))
		errno = 0;
	return (join_path);
}

static char	*check_path_list(char **env_path, char *cmd)
{
	int		i;
	char	*join_path;
	char	*save_error_path;

	save_error_path = NULL;
	i = -1;
	while (env_path[++i] != NULL)
	{
		join_path = create_and_check_path(env_path[i], cmd);
		if (errno == 0)
		{
			free(save_error_path);
			return (join_path);
		}
		else if (errno == EACCES)
		{
			free(save_error_path);
			save_error_path = ft_strdup(join_path);
			if (save_error_path == NULL)
				err_exit("malloc error: ");
		}
		free(join_path);
	}
	return (save_error_path);
}

char	*check_path(char *path)
{
	char	*env_path;
	char	*join_path;
	char	**split_path;

	env_path = search_key(g_sh_var.environ, "PATH");
	if (env_path == NULL)
		return (NULL);
	split_path = ft_split(env_path, ':');
	join_path = check_path_list(split_path, path);
	free_strs(split_path);
	if (errno == 0)
		return (join_path);
	errno = ENOENT;
	if (join_path)
		errno = EACCES;
	return (join_path);
}

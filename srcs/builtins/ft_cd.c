#include "minishell.h"

char	*no_current_dir(char *pwd, char *path)
{
	ft_putendl_fd("cd: error retrieving current directory: \
		getcwd: cannot access parent directories: No such file or directory", STDERR_FILENO);
	free(pwd);
	pwd = ft_strjoin(pwd, path);
	return (pwd);
}

void	set_pwd(t_env *env, char *path)
{
	char	*pwd;
	char	*oldpwd;

	printf("[bef]pwd: %s\n", search_key(env, "PWD"));
	printf("[bef]oldpwd: %s\n", search_key(env, "OLDPWD"));

	char *tmp;
	// getcwdでセットされたerrnoを見ると、cdやcd -のケースの時にセットされている
	tmp = getcwd(NULL, 0); 
	printf("pwd: %s\n", tmp);
	if (tmp == NULL)
		tmp = no_current_dir(tmp, path);
	update_or_add_value(env, "OLDPWD", search_key(env, "PWD"));
	pwd = ft_strdup(tmp);
	free(tmp);
	if (pwd == NULL)
		return ;
	update_or_add_value(env, "PWD", pwd);
	printf("[aft]pwd: %s\n", search_key(env, "PWD"));
	printf("[aft]oldpwd: %s\n", search_key(env, "OLDPWD"));
}

int	get_path(t_env *env, char *s, char **path)
{
	if (s == NULL)
		*path = ft_strdup(search_key(env, "HOME"));
	else if (ft_strlen(s) == 1 && ft_strncmp(s, "-", 1) == 0)
		*path = ft_strdup(search_key(env, "OLDPWD"));
	else
		*path = ft_strdup(s);
	if ((s == NULL || ft_strncmp(s, "-", 2) == 0) && !(*path))
	{
		if (s == NULL)
			*path = ft_strdup("HOME");
		else
			*path = ft_strdup("OLDPWD");
	}
	return -1 * (*path == NULL);
}

void	print_error(char *dir, char *msg)
{
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
		ft_putstr_fd(dir, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putendl_fd(msg, STDERR_FILENO);
}

int	do_cd(char *path)
{
	int	status;

	status = chdir(path);
	if (status == -1)
	{
		print_error(path, "no such file or directory");
		return (1);
	}
	return (0);
}

int	ft_cd(t_env *env, char **strs)
{
	int	status;
	char	*path;

	status = get_path(env, strs[1], &path);
	if (status == -1)
	{
		print_error("", "not set");
		return (0);
	}
	//printf("path: %s\n", path);
	status = do_cd(path);
	printf("status: %d\n", status);
	if (!status)
		set_pwd(env, strs[1]);
	return (1);
}

void	print_cmds(char **strs)
{
	int	i;

	i = 0;
	printf("\npath: [");
	while (strs[i])
		printf("%s ", strs[i++]);
	printf("]\n");
}

int main(void)
{
	t_env	*env;
	char *strs[3] = {"cd", "./libft"};
	char *strs2[3] = {"cd", "/Users/akiyama/code/42/minishell"};
	char *strs3[3] = {"cd", "../"};
	char *strs4[3] = {"cd", "/akiyama/code/42/minishell"}; // error
	char *strs5[3] = {"cd"};
	char *strs6[3] = {"cd", "-"}; // OLDPWD
	char *strs7[3] = {"cd", ".test/libft"}; // symbolic link
	char *strs8[3] = {"cd", "/Users/akiyama/code/42/minishell/sample"};
	char *strs9[3] = {"cd", "./"};
	char *tmp;

	env = create_env();
	print_cmds(strs);
	tmp = getcwd(NULL, 0);
	printf("%s\n", tmp);
	free(tmp);
	ft_cd(env, strs);
	tmp = getcwd(NULL, 0);
	printf("%s\n\n\n", tmp);
	free(tmp);

	print_cmds(strs2);
	tmp = getcwd(NULL, 0);
	printf("%s\n", tmp);
	free(tmp);
	ft_cd(env, strs2);
	tmp = getcwd(NULL, 0);
	printf("%s\n\n\n", tmp);
	free(tmp);

	tmp = getcwd(NULL, 0);
	print_cmds(strs3);
	free(tmp);
	printf("%s\n", tmp);
	ft_cd(env, strs3);
	tmp = getcwd(NULL, 0);
	printf("%s\n\n\n", tmp);
	free(tmp);

	print_cmds(strs4);
	tmp = getcwd(NULL, 0);
	free(tmp);
	printf("%s\n", tmp);
	ft_cd(env, strs4);
	tmp = getcwd(NULL, 0);
	printf("%s\n\n\n", tmp);
	free(tmp);

	print_cmds(strs5);
	tmp = getcwd(NULL, 0);
	printf("%s\n", tmp);
	free(tmp);
	ft_cd(env, strs5);
	tmp = getcwd(NULL, 0);
	printf("%s\n\n\n", tmp);
	free(tmp);

	print_cmds(strs6);
	tmp = getcwd(NULL, 0);
	printf("%s\n", tmp);
	free(tmp);
	ft_cd(env, strs6);
	tmp = getcwd(NULL, 0);
	printf("%s\n\n\n", tmp);
	free(tmp);

	print_cmds(strs7);
	tmp = getcwd(NULL, 0);
	printf("%s\n", tmp);
	free(tmp);
	ft_cd(env, strs7);
	tmp = getcwd(NULL, 0);
	printf("%s\n\n\n", tmp);
	free(tmp);

	print_cmds(strs8);
	tmp = getcwd(NULL, 0);
	printf("%s\n", tmp);
	free(tmp);
	ft_cd(env, strs8);
	system("rm -r sample");
	ft_cd(env, strs9);
	tmp = getcwd(NULL, 0);
	printf("%s\n\n\n", tmp);
	free(tmp);
	return (0);
}

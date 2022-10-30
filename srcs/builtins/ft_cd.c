#include "minishell.h"

//t_env	*g_sh_var.environ;

char	*join_with_connector(char *s1, char *s2, char connector)
{
	size_t	len1;
	size_t	len2;
	char	*s;
	size_t	i;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	s = (char *)malloc(sizeof(char) * (len1 + len2) + 2);
	if (s == NULL)
		err_exit("malloc error: ");
	i = 0;
	while (i < len1)
	{
		s[i] = s1[i];
		i++;
	}
	s[i++] = connector;
	while (i < len1 + len2 + 1)
	{
		s[i] = s2[i - len1 - 1];
		i++;
	}
	s[i] = '\0';
	return (s);
}

//char	*no_current_dir(t_env *env, char *path)
char	*no_current_dir(char *path)
{
	char	*pwd;
	char	*newpwd;

	ft_putendl_fd("cd: error retrieving current directory: \
		getcwd: cannot access parent directories: No such file or directory", STDERR_FILENO);
	
	//pwd = search_key(env, "PWD");
	pwd = search_key(g_sh_var.environ, "PWD");
	newpwd = join_with_connector(pwd, path, '/');
	printf("pwd:: %s\n", newpwd);
	return (newpwd);
}

//void	set_pwd(t_env *env, char *path)
void	set_pwd(char *path)
{
	char	*pwd;

	pwd = getcwd(NULL, 0); 
	if (pwd == NULL)
		//pwd = no_current_dir(env, path);
		pwd = no_current_dir(path);
	//update_or_add_value(&env, "OLDPWD", search_key(env, "PWD"));
	//update_or_add_value(&env, "PWD", pwd);
	update_or_add_value(&g_sh_var.environ, "OLDPWD", search_key(g_sh_var.environ, "PWD"));
	printf("ok\n");
	update_or_add_value(&g_sh_var.environ, "PWD", pwd);
	free(pwd);
}

//int	get_path(t_env *env, char *s, char **path)
int	get_path(char *s, char **path)
{
	int	status;

	status = -1;
	if (s == NULL)
		*path = ft_xstrdup(search_key(g_sh_var.environ, "HOME"));
	else if (ft_strncmp(s, "-", 2) == 0)
		*path = ft_xstrdup(search_key(g_sh_var.environ, "OLDPWD"));
	else
		*path = ft_xstrdup(s);
	if (*path != NULL)
		status = 0;
	if (*path == NULL && s == NULL)
		*path = ft_xstrdup("HOME");
	if (*path == NULL && ft_strncmp(s, "-", 2) == 0)
		*path = ft_xstrdup("OLDPWD");
	return (status);
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
		print_error(path, "No such file or directory");
		return (1);
	}
	return (0);
}

int	ft_cd(char **strs)//, t_env *env)
{
	int	status;
	char	*path;

	//status = get_path(g_sh_var.environ, strs[1], &path);
	status = get_path(strs[1], &path);
	if (status == -1)
	{
		print_error(path, "not set");
		free(path);
		return (0);
	}
	status = do_cd(path);
	if (!status)
		//set_pwd(g_sh_var.environ, strs[1]);
		set_pwd(strs[1]);
	free(path);
	return (1);
}
/*
void	print_cmds(char **strs)
{
	int	i;

	i = 0;
	printf("\npath: [");
	while (strs[i])
		printf("%s ", strs[i++]);
	printf("]\n");
}
*/
/*
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
	char *strs10[3] = {"cd", "$HOME"};
	char *tmp;

	system("export $HOME=/Users/akiyama");


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

	print_cmds(strs10);
	tmp = getcwd(NULL, 0);
	printf("%s\n", tmp);
	free(tmp);
	system("unset HOME");
	ft_cd(env, strs10);
	tmp = getcwd(NULL, 0);
	printf("%s\n\n\n", tmp);
	free(tmp);
	print_cmds(strs8);
	tmp = getcwd(NULL, 0);
	printf("%s\n", tmp);
	free(tmp);

	system("mkdir /Users/akiyama/code/42/minishell/sample");
	ft_cd(env, strs8);
	system("rm -r /Users/akiyama/code/42/minishell/sample");
	ft_cd(env, strs9);
	tmp = getcwd(NULL, 0);
	printf("%s\n\n\n", tmp);
	free(tmp);

	print_cmds(strs3);
	tmp = getcwd(NULL, 0);
	printf("%s\n", tmp);
	free(tmp);
	ft_cd(env, strs3);
	tmp = getcwd(NULL, 0);
	printf("%s\n\n\n", tmp);
	free(tmp);

	return (0);
}
*/

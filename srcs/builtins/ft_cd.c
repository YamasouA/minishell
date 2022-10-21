#include "minishell.h"

//t_env	*g_environ;

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
		return (NULL);
	i = 0;
	while (i < len1)
	{
		s[i] = s1[i];
		i++;
	}
//	s[i++] = '/';
	s[i++] = connector;
	while (i < len1 + len2 + 1)
	{
		s[i] = s2[i - len1 - 1];
		i++;
	}
	s[i] = '\0';
	return (s);
}
/*
char	*join_slash(char *s1, char *s2)
{
	size_t	len1;
	size_t	len2;
	char	*s;
	size_t	i;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	s = (char *)malloc(sizeof(char) * (len1 + len2) + 2);
	i = 0;
	while (i < len1)
	{
		s[i] = s1[i];
		i++;
	}
	s[i++] = '/';
	while (i < len1 + len2 + 1)
	{
		s[i] = s2[i - len1 - 1];
		i++;
	}
	s[i] = '\0';
	return (s);
}
*/

char	*no_current_dir(t_env *env, char *path)
{
	char	*pwd;
	char	*newpwd;

	ft_putendl_fd("cd: error retrieving current directory: \
		getcwd: cannot access parent directories: No such file or directory", STDERR_FILENO);
	
	pwd = search_key(env, "PWD");
	//newpwd = ft_strjoin(pwd, "/");
	//newpwd = ft_strjoin(pwd, path);
	//newpwd = join_slash(pwd, path);
	newpwd = join_with_connector(pwd, path, '/');
	printf("pwd:: %s\n", newpwd);
	return (newpwd);
}

void	set_pwd(t_env *env, char *path)
{
	char	*pwd;
	//char	*oldpwd;

	//printf("[bef]pwd: %s\n", search_key(env, "PWD"));
	//printf("[bef]oldpwd: %s\n", search_key(env, "OLDPWD"));

	char *tmp;
	// getcwdでセットされたerrnoを見ると、cdやcd -のケースの時にセットされている
	tmp = getcwd(NULL, 0); 
	if (tmp == NULL)
		tmp = no_current_dir(env, path);
	//printf("pwd: %s\n", tmp);
	update_or_add_value(env, "OLDPWD", search_key(env, "PWD"));
	pwd = ft_strdup(tmp);
	free(tmp);
	if (pwd == NULL)
		return ;
	update_or_add_value(env, "PWD", pwd);
	//printf("[aft]pwd: %s\n", search_key(env, "PWD"));
	//printf("[aft]oldpwd: %s\n", search_key(env, "OLDPWD"));
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

int	ft_cd(char **strs)//, t_env *env)
{
	int	status;
	char	*path;

	status = get_path(g_environ, strs[1], &path);
	if (status == -1)
	{
		print_error("", "not set");
		return (0);
	}
	//printf("path: %s\n", path);
	status = do_cd(path);
	printf("status: %d\n", status);
	if (!status)
		set_pwd(g_environ, strs[1]);
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

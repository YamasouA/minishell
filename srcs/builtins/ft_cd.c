#include "minishell.h"

static void	set_pwd(char *path)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (pwd == NULL)
		pwd = no_current_dir(path);
	update_or_add_value(&g_sh_var.environ, "OLDPWD", \
		search_key(g_sh_var.environ, "PWD"));
	update_or_add_value(&g_sh_var.environ, "PWD", pwd);
	free(pwd);
}

static int	get_path(char *s, char **path)
{
	int	status;

	status = -1;
	if (*s == '\0')
		*path = ft_xstrdup(search_key(g_sh_var.environ, "HOME"));
	else if (ft_strncmp(s, "-", 2) == 0)
		*path = ft_xstrdup(search_key(g_sh_var.environ, "OLDPWD"));
	else
		*path = ft_xstrdup(s);
	if (*path != NULL)
		status = 0;
	if (*path == NULL && *s == '\0')
		*path = ft_xstrdup("HOME");
	if (*path == NULL && ft_strncmp(s, "-", 2) == 0)
		*path = ft_xstrdup("OLDPWD");
	return (status);
}

static int	do_cd(char *path)
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

int	ft_cd(char **strs)
{
	int		status;
	char	*path;

	if (strs[1] && (strs[1][0] == '-' && strs[1][1]))
	{
		print_error_and_usage(strs[1], "cd");
		return (2);
	}
	status = get_path(strs[1], &path);
	if (status == -1)
	{
		print_error(path, "not set");
		free(path);
		return (1);
	}
	status = do_cd(path);
	if (!status)
		set_pwd(strs[1]);
	free(path);
	return (status);
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

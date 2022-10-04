#include "minishell.h"

void	set_pwd(char *newpwd)
{
	char	*pwd;
	char	*oldpwd;

	oldpwd = getenv("OLDPWD");
	pwd = getenv("PWD");

	printf("[bef]pwd: %s\n", pwd);
	printf("[bef]oldpwd: %s\n", oldpwd);

	char *tmp;
	oldpwd = pwd;
	tmp = getcwd(NULL, 0);
	pwd = tmp;
	free(tmp);
	printf("[aft]pwd: %s\n", pwd);
	printf("[aft]oldpwd: %s\n", oldpwd);
}

void	ft_cd(char **strs)
{
	int	status;
	char	*path;

	if (strs[1] == NULL)
		path = getenv("HOME");
	else
		path = strs[1];
	status = chdir(path);
	printf("status: %d\n", status);
	if (!status)
		set_pwd(strs[1]);
	else
	{
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
		ft_putstr_fd(strs[1], STDERR_FILENO);
		ft_putendl_fd(": no such file or directory", STDERR_FILENO);
	}
}

void	print_cmds(char **strs)
{
	int	i;

	i = 0;
	printf("path: ");
	while (strs[i])
		printf("%s ", strs[i++]);
	printf("\n");
}

int main(void)
{

	char *strs[3] = {"cd", "./libft"};
	char *strs2[3] = {"cd", "/Users/akiyama/code/42/minishell"};
	char *strs3[3] = {"cd", "../"};
	char *strs4[3] = {"cd", "/akiyama/code/42/minishell"}; //error
	char *strs5[3] = {"cd"};
	char *strs6[3] = {"cd", ".test/libft"}; // symbolic link
	char *tmp;
	print_cmds(strs);
	tmp = getcwd(NULL, 0);
	printf("%s\n", tmp);
	free(tmp);
	ft_cd(strs);
	tmp = getcwd(NULL, 0);
	printf("%s\n\n\n", tmp);
	free(tmp);

	print_cmds(strs2);
	tmp = getcwd(NULL, 0);
	printf("%s\n", tmp);
	free(tmp);
	ft_cd(strs2);
	tmp = getcwd(NULL, 0);
	printf("%s\n\n\n", tmp);
	free(tmp);

	tmp = getcwd(NULL, 0);
	print_cmds(strs3);
	free(tmp);
	printf("%s\n", tmp);
	ft_cd(strs3);
	tmp = getcwd(NULL, 0);
	printf("%s\n\n\n", tmp);
	free(tmp);

	print_cmds(strs4);
	tmp = getcwd(NULL, 0);
	free(tmp);
	printf("%s\n", tmp);
	ft_cd(strs4);
	tmp = getcwd(NULL, 0);
	printf("%s\n\n\n", tmp);
	free(tmp);

	print_cmds(strs5);
	tmp = getcwd(NULL, 0);
	printf("%s\n", tmp);
	free(tmp);
	ft_cd(strs5);
	tmp = getcwd(NULL, 0);
	printf("%s\n\n\n", tmp);
	free(tmp);

	print_cmds(strs6);
	tmp = getcwd(NULL, 0);
	printf("%s\n", tmp);
	free(tmp);
	ft_cd(strs6);
	tmp = getcwd(NULL, 0);
	printf("%s\n\n\n", tmp);
	free(tmp);
	return (0);
}

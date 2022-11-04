#include "minishell.h"
#include <dirent.h>

t_sh_var	g_sh_var={};

void	set_signal_handler(int signum, sig_t sighandler)
{
	if (signal(signum, sighandler) == SIG_ERR)
		err_exit("signal error: ");
}
char	*add_redirect_out(char *line, char *output)
{
	char	*new;
	new = ft_strjoin(line, output);
	return (new);
}

static void tester(char *file_name)
{
	int fd;
	char *line;
	t_token	*tok;
	t_node	*node;
	int	line_n;
	bool	heredoc_err;
	char	*line2;
	char	*line3;
	if (system(NULL) == 0)
		return ;
	printf("\n\n\nfile_name: %s\n", file_name);
	g_sh_var.environ = create_env();
	//file_name = "test.txt";
//	file_name = "test3.txt";
	fd = open(file_name, O_RDONLY);
	if (fd == -1)
	{
		printf("can't open file\n");
		return ;
	}
	line = NULL;
	line_n = 1;
	do {
		errno = 0;
		free(line);
		line = get_next_line(fd);
		if (line == NULL)
			break;
		line[ft_strlen(line) - 1] = '\0';
		system("> minishell.txt");
		system("> bash.txt");
		line2 = add_redirect_out(line, ">minishell.txt");
		printf("\n============%s: line %d=============\n", file_name, line_n);
		//printf("%s\n", line2);
//		printf("cmd: %s", line);
		ft_putstr_fd("cmd: ", 1);
		ft_putstr_fd(line, 1);
		ft_putstr_fd("\n", 1);
		line_n++;
		if (line[0] == '/' && line[1] == '/')
			continue;
		//printf("input line: %s\n", line);
		tok = lexer(line2);
		if (tok == NULL)
			continue ;
		node = parse(tok, &heredoc_err);
		if (node == NULL)
		{
			continue;
		}
		//printf("==PARSE==\n");
		//print_node(node, 0);
		node = expansion(node);
		//printf("==EXPANSION==\n");
		//print_node(node, 0);
		exec(node, 0);
		line3 = ft_strjoin("echo \"", line);
		line3 = ft_strjoin(line3, "\"");
		line3 = add_redirect_out(line3, "| bash > bash.txt");
		//printf("%s\n", line3);
		system(line3);
//		system("diff -s minishell.txt bash.txt >> diff.txt");
		system("diff -s minishell.txt bash.txt | grep -v identical");
//		system("cat diff.txt");
//		system("cat diff.txt | grep -v identical ; if [ $? = 0 ];then cat diff.txt >> result.txt;fi");
	} while (1);
	free(line);
	close(fd);
}

int main(int argc, char *argv[])
{
    DIR *dir;
    struct dirent *dp;
    char dirpath[] = ".test/";
    int	i;

    i = 0;
    dir = NULL;
    if (argc > 1)
    {
    	i = 1;
	while (argv[i] != NULL)
	{
	    tester(ft_strjoin(dirpath, argv[i]));
	    i++;
	}
    }
    else {
        dir = opendir(dirpath);
    	if (dir == NULL) { return 1; }
    	dp = readdir(dir);
    	while (dp != NULL) {
    	    printf("%s\n", dp->d_name);
	    if (dp->d_name[0] != '.')
	    	tester(ft_strjoin(dirpath, dp->d_name));
    	    dp = readdir(dir);
    	}

    }


    if (dir != NULL) { closedir(dir); }

    return 0;
} 

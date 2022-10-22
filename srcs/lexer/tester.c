#include "minishell.h"
#include <dirent.h>

t_env	*g_environ;
bool	g_signal;

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
	//char *file_name;
	char *line;
	t_token	*tok;
	t_node	*node;
	int	line_n;
	bool	heredoc_err;
	char	*line2;
	char	*line3;
//	t_env	*environ;
	
	printf("file_name: %s\n", file_name);
	g_environ = create_env();
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
		free(line);
		line = get_next_line(fd);
		if (line == NULL)
			break;
		line2 = add_redirect_out(line, ">minishell.txt");
		printf("\n============%s: line %d=============\n", file_name, line_n);
		line_n++;
		if (line[0] == '/' && line[1] == '/')
			continue;
		//printf("input line: %s\n", line);
		line[ft_strlen(line) - 1] = '\0';
		tok = lexer(line);
		node = parse(tok, &heredoc_err);
		//printf("==PARSE==\n");
		//print_node(node, 0);
		node = expansion(node);
		//printf("==EXPANSION==\n");
		//print_node(node, 0);
		exec(node, 0);
		line3 = add_redirect_out(line, ">bash.txt");
		system(line3);
		system("diff minishell.txt bash.txt");
	} while (1);
	free(line);
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

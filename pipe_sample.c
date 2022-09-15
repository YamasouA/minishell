#include <unistd.h>
#include <sys/wait.h>

char *cmd1[] = {"cat", "pipe_sample.c", NULL};
char *cmd2[] = {"ls", NULL};
char *cmd3[] = {"grep", "char", NULL};
char **cmds[] = {cmd1, cmd2, cmd3};
int cmd_n = 3;

void dopipes(int i) {
	pid_t ret;
	int pp[2] = {};

	if (i == cmd_n - 1) {
		execvp(cmds[0][0], cmds[0]);
	}
	else {
		pipe(pp);
		ret = fork();

		if (ret == 0) {
			close(pp[0]);
			dup2(pp[1], 1);
			close(pp[1]);

			dopipes(i + 1);
		}
		else {
			close(pp[1]);
			dup2(pp[0], 0);
			close(pp[0]);

			execvp(cmds[cmd_n-i-1][0], cmds[cmd_n-i-1]);
		}
	}
}

int main(void) {
	pid_t ret;

	ret = fork();
	if (ret == 0)
		dopipes(0);
	else
		wait(NULL);
	return 0;
}

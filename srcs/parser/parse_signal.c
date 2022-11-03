#include "minishell.h"

void	heredoc_signal_handler(int sig)
{
	g_sh_var.signal = sig;
}

int	check_state(void)
{
	if (g_sh_var.signal != 0)
	{
		//g_sh_var.signal = 0;
		rl_done = 1;
	}
	return (0);
}

void	processing_on_signal(char *line, char *documents, bool *heredoc_err)
{
	*heredoc_err = 1;
	set_signal_handler(SIGINT, SIG_IGN);
	free(line);
	free(documents);
}

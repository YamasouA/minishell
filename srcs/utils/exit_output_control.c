#include "minishell.h"

static int	get_x_pos(void)
{
	int		i;	
	char	c;
	char	p[32];
	char	*semic_pos;

	i = 0;
	while (read(0, &c, 1) != -1)
	{
		p[i++] = c;
		if (c == 'R')
			break ;
	}
	semic_pos = ft_strchr(p, ';');
	return (ft_atoi(semic_pos + 1));
}

int	get_print_start(void)
{
	int			x;
	t_termios	oldstate;
	t_termios	state;

	if (tcgetattr(0, &oldstate) == -1)
		return (0);
	state = oldstate;
	state.c_lflag &= ~(ICANON | ECHO);
	if (tcsetattr(0, TCSANOW, &state) == -1)
		return (0);
	ft_putstr_fd("\e[6n", 0);
	x = get_x_pos();
	if (tcsetattr(0, TCSANOW, &oldstate) == -1)
		return (0);
	return (x - 1);
}

void	display_exit(int x)
{
	struct winsize	ws;
	int				line_length;

	line_length = 0;
	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1) //cut func ?
		return ;
	if (0 < ws.ws_col && ws.ws_col == (int)ws.ws_col)
		line_length = (int)ws.ws_col;
	if (line_length - PROMPT_LENGTH - x < 0)
	{
		x = (line_length - PROMPT_LENGTH - x) * (-1);
		printf("\e[1A\e[%dCexit\n", x);
	}
	else if (line_length - PROMPT_LENGTH - x == 0)
		printf("exit\n");
	else
		printf("\e[1A\e[%dCexit\n", x + PROMPT_LENGTH);
}

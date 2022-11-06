/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_output_control.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asouta <asouta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 22:55:47 by shongou           #+#    #+#             */
/*   Updated: 2022/11/05 23:33:25 by asouta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_x_pos(void)
{
	int		i;
	char	c;
	char	p[32];
	char	*semic_pos;

	i = 0;
	while (read(STDIN_FILENO, &c, 1) != -1)
	{
		p[i++] = c;
		if (c == 'R')
			break ;
	}
	p[i] = '\0';
	semic_pos = ft_strchr(p, ';');
	return (ft_atoi(semic_pos + 1));
}

static int	get_window_width(void)
{
	struct winsize	ws;
	int				line_length;

	line_length = 0;
	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1)
		return (-1);
	if (0 < ws.ws_col && ws.ws_col == (int)ws.ws_col)
		line_length = (int)ws.ws_col;
	return (line_length);
}

int	get_print_start(void)
{
	int			x;
	t_termios	oldstate;
	t_termios	state;

	if (tcgetattr(STDIN_FILENO, &oldstate) == -1)
		return (0);
	state = oldstate;
	state.c_lflag &= ~(ICANON | ECHO);
	if (tcsetattr(STDIN_FILENO, TCSANOW, &state) == -1)
		return (0);
	ft_putstr_fd("\e[6n", STDIN_FILENO);
	x = get_x_pos();
	if (tcsetattr(STDIN_FILENO, TCSANOW, &oldstate) == -1)
		return (0);
	return (x - 1);
}

void	display_exit(int x)
{
	int				line_length;
	char			*exit_str;

	line_length = get_window_width();
	if (x + 1 == line_length)
		x = 0;
	if (line_length - PROMPT_LENGTH - x < 0)
	{
		x = (line_length - PROMPT_LENGTH - x) * (-1);
		exit_str = ft_joinfree(ft_strdup("\e[1A\e["), ft_itoa(x));
		exit_str = ft_joinfree(exit_str, ft_strdup("Cexit\n"));
		ft_putstr_fd(exit_str, STDERR_FILENO);
		free(exit_str);
	}
	else if (line_length - PROMPT_LENGTH - x == 0)
		ft_putstr_fd("exit\n", STDERR_FILENO);
	else
	{
		exit_str = ft_joinfree(ft_strdup("\e[1A\e["), ft_itoa(x + PROMPT_LENGTH));
		exit_str = ft_joinfree(exit_str, ft_strdup("Cexit\n"));
		ft_putstr_fd(exit_str, STDERR_FILENO);
		free(exit_str);
	}
}

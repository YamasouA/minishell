/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shongou <shongou@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 23:02:59 by shongou           #+#    #+#             */
/*   Updated: 2022/11/04 23:03:02 by shongou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_sh_var	g_sh_var = {};

static void	signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		ft_putchar_fd('\n', 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	init(char **line, t_token **tok, t_node **node)
{
	char	*pwd;

	errno = 0;
	g_sh_var.signal = 0;
	g_sh_var.environ = create_env();
	pwd = getcwd(NULL, 0);
	update_or_add_value(&g_sh_var.environ, "PWD", pwd);
	free(pwd);
	update_or_add_value(&g_sh_var.environ, "OLDPWD", NULL);
	*line = NULL;
	*tok = NULL;
	*node = NULL;
}

char	*readline_wrapper(int *x)
{
	char	*line;

	*x = get_print_start();
	line = readline("minishell> ");
	if (line == NULL)
	{
		display_exit(*x);
		exit(0);
	}
	if (ft_strlen(line) != 0)
		add_history(line);
	return (line);
}

void	loop_init(bool *heredoc_err, char **line, t_token **tok, t_node **node)
{
	set_signal_handler(SIGINT, signal_handler);
	set_signal_handler(SIGQUIT, SIG_IGN);
	all_free(*line, *tok, *node);
	errno = 0;
	g_sh_var.signal = 0;
	*heredoc_err = 0;
	*line = NULL;
	*tok = NULL;
	*node = NULL;
}

void	minishell(void)
{
	char	*line;
	t_token	*tok;
	t_node	*node;
	bool	heredoc_err;
	int		x;

	init(&line, &tok, &node);
	while (1)
	{
		loop_init(&heredoc_err, &line, &tok, &node);
		line = readline_wrapper(&x);
		set_signal_handler(SIGINT, SIG_IGN);
		if (ft_strlen(line) == 0)
			continue ;
		tok = lexer(line);
		if (tok == NULL)
			continue ;
		node = parse(&tok, &heredoc_err);
		if (g_sh_var.signal != 0 || node == NULL)
			continue ;
		node = expansion(node);
		exec(node, 0);
	}
}

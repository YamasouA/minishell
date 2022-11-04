/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shongou <shongou@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 22:54:00 by shongou           #+#    #+#             */
/*   Updated: 2022/11/04 22:54:01 by shongou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*tokenize_error(char token, t_token *head)
{
	g_sh_var.exit_status = 258;
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	ft_putchar_fd(token, 2);
	ft_putstr_fd("'\n", 2);
	free_token(head->next);
	return (NULL);
}

t_token	*quote_error(char *token, t_token *head)
{
	char	*tmp;

	g_sh_var.exit_status = 258;
	ft_putstr_fd("minishell: unexpected EOF while looking for matching `", 2);
	while (*token != '\0')
	{
		if (*token == '\'' || *token == '"')
		{
			tmp = token;
			token = find_quote(token + 1, *token);
			if (token == NULL)
			{
				ft_putchar_fd(tmp[0], 2);
				break ;
			}
		}
		token++;
	}
	ft_putstr_fd("'\n", 2);
	free_token(head->next);
	return (NULL);
}

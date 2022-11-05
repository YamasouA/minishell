/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asouta <asouta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 22:54:00 by shongou           #+#    #+#             */
/*   Updated: 2022/11/05 23:27:36 by asouta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*tokenize_error(char token, t_token *head)
{
	g_sh_var.exit_status = 258;
	ft_putstr_fd("minishell: syntax error near unexpected token `", STDERR_FILENO);
	ft_putchar_fd(token, STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
	free_token(head->next);
	return (NULL);
}

t_token	*quote_error(char *token, t_token *head)
{
	char	*tmp;

	g_sh_var.exit_status = 258;
	ft_putstr_fd("minishell: unexpected EOF while looking for matching `", STDERR_FILENO);
	while (*token != '\0')
	{
		if (*token == '\'' || *token == '"')
		{
			tmp = token;
			token = find_quote(token + 1, *token);
			if (token == NULL)
			{
				ft_putchar_fd(tmp[0], STDERR_FILENO);
				break ;
			}
		}
		token++;
	}
	ft_putstr_fd("'\n", STDERR_FILENO);
	free_token(head->next);
	return (NULL);
}

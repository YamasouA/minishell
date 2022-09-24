#include "minishell.h"

t_node	*new_binary(t_node_kind kind, t_node *lhs, t_node *rhs, t_token *tok)
{

}

t_node	*new_unary(t_node_kind kind, t_node *expr, t_token *tok)
{

}

t_node	*io_file()
{
	if ( == '<')
	{
		filename();
	}
	else if (== '>')
	{
		filename();
	}
	else if ( == ">>")
	{
		filename();
	}
	else
		error("OUT!!!");
	return (node);
}

t_node *io_here()
{
	if (== "<<")
	{
		here_end;
	}
	else
		error("OUT!!!");
	return (node);
}

t_node	*io_redirect()
{
	if (is_io_file())
	{
	}
	else if(is_io_here())
	{
	}
	else
		error("OUT!!!");
	return (node);
}

t_node *cmd_suffix()
{
	int	cnt;

	cnt = 0;
	while (1)
	{
		if (is_io_redirect())
		{
			io_redirect();
		}
		else if (WORD)
		{
		}
		else if (cnt == 0)
			error("OUT!!!");
		else
			break ;
		cnt = 1;
	}
	return (node);
}

t_node	*command()
{
	int	cnt;

	cnt = 0;
	while (1)
	{
		if (is_io_redirect())
		{
		}
		else if (ASIIGNMENT_WORD)
		{
		}
		else if (cnt == 0)
			error("OUT!!!");
		else
			break ;
		cnt = 1;
	}
	if (is_cmd_word())
	{
		if (is_cmd_suffix())
		{
		}
	}
	else if(is_cmd_name())
	{
		if (is_cmd_suffix())
	}
	else
		error("OUT!!!");
	return (node);
}

t_node	*complete_commands()
{
	t_node	*node;
	t_token	*tok;

	node = command();
	while (tok = consume("|"))
	{
		node = new_unary(ND_COMMAND, node, node->tok);
		node = new_binary(ND_PIPE, node, command(), tok);
	}
	return (node);
}

t_node	*program()
{
	t_node	*node;

	if (is_complete_commands())
	{
		complete_commands();
	}
	return (node);
}

void parse(t_token list)
{
	program();
}

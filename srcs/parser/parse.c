#include "minishell.h"

t_node	*new_binary(t_node_kind kind, t_node *lhs, t_node *rhs, t_token *tok)
{

}

t_node	*new_unary(t_node_kind kind, t_node *expr, t_token *tok)
{

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
		cnt++;
	}
	if (is_cmd_word
	
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

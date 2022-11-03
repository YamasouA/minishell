#incllude "minishell.h"

bool	is_valid_file_name(t_redirect *redirect)
{
	if (redirect->type == REDIRECT_NONE)
		return (print_redirect_err(redirect->file_name));
	else if (redirect->type == APPEND || redirect->type == REDIRECT_OUT)
	{
		if ((access(redirect->file_name, W_OK) && errno == EACCES)
			|| redirect->file_name[0] == '\0')
			return (print_redirect_err(redirect->file_name));
		else if (is_directory(redirect->file_name))
		{
			//ft_putstr_fd("minishell: ", 2);
			//ft_putstr_fd(redirect->file_name, 2);
			//ft_putstr_fd(": is a directory\n", 2);
			err_is_a_directory(redirect->file_name);
			return (0);
		}
	}
	else
	{
		if (access(redirect->file_name, R_OK))
			return (print_redirect_err(redirect->file_name));
	}
	return (1);
}

int	open_and_dup2(t_redirect *redirect)
{
	int	fd;

	if (!is_valid_file_name(redirect))
		return (1);
	if (redirect->type == APPEND)
	{
		fd = open(redirect->file_name, O_CREAT | O_WRONLY | O_APPEND, 0644);
		xdup2(fd, 1);
	}
	else if (redirect->type == REDIRECT_OUT)
	{
		fd = open(redirect->file_name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		xdup2(fd, 1);
	}
	else
	{
		fd = open(redirect->file_name, O_RDONLY);
		xdup2(fd, 0);
		if (redirect->type == HEREDOC)
			xunlink(redirect->file_name);
	}
	xclose(fd);
	return (0);
}

void	create_heredoc_tmpfile(t_redirect *redirect_in)
{
	int		fd;

//	flag = O_CREAT | O_WRONLY | O_TRUNC;
	fd = open(redirect_in->file_name, O_CREAT | O_WRONLY | O_TRUNC, 00644);
	ft_putstr_fd(redirect_in->documents, fd);
	free(redirect_in->documents);
	redirect_in->documents = NULL;
	xclose(fd);
}

int	do_redirect(t_cmd *cmd)
{
	t_redirect	*redirect_in;
	t_redirect	*redirect_out;

	redirect_in = cmd->redirect_in;
	while (redirect_in->next)
	{
		redirect_in = redirect_in->next;
		if (redirect_in->type == HEREDOC)
		{
			create_heredoc_tmpfile(redirect_in);
		}
		if (open_and_dup2(redirect_in))
			return (1);
	}
	redirect_out = cmd->redirect_out;
	while (redirect_out->next)
	{
		redirect_out = redirect_out->next;
		if (open_and_dup2(redirect_out))
			return (1);
	}
	return (0);
}

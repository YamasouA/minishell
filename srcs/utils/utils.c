#include "minishell.h"

char	*join_with_connector(char *s1, char *s2, char connector)
{
	size_t	len1;
	size_t	len2;
	char	*s;
	size_t	i;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	s = (char *)malloc(sizeof(char) * (len1 + len2) + 2);
	if (s == NULL)
		err_exit("malloc error: ");
	i = 0;
	while (i < len1)
	{
		s[i] = s1[i];
		i++;
	}
	s[i++] = connector;
	while (i < len1 + len2 + 1)
	{
		s[i] = s2[i - len1 - 1];
		i++;
	}
	s[i] = '\0';
	return (s);
}

void	set_signal_handler(int signum, sig_t sighandler)
{
	if (signal(signum, sighandler) == SIG_ERR)
		err_exit("signal error: ");
}

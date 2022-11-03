#include "libft.h"

char	*ft_xstrdup(const char *s1)
{
	size_t	len;
	char	*sp;

	if (s1 == NULL)
		return (NULL);
	len = ft_strlen(s1);
	sp = (char *)malloc(len + 1);
	if (sp == NULL)
	{
		ft_putendl_fd("malloc error", 2);
		exit(1);
	}
	ft_strlcpy(sp, s1, len + 1);
	return (sp);
}

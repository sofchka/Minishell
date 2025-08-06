#include "minishell.h"

int	has_open_quote(const char *s)
{
	int		i;
	char	q;

	i = 0;
	while (s[i])
	{
		if (s[i] == '\'' || s[i] == '"')
		{
			q = s[i];
			i++;
			while (s[i] && s[i] != q)
				i++;
			if (!s[i])
				return (1);
		}
		i++;
	}
	return (0);
}

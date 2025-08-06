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
			{
				write(2, "Syntax error: unclosed quote.\n", 31);
				return (1);
			}
		}
		i++;
	}
	return (0);
}

static int	skip_spaces(const char *s, int *i)
{
	while (s[*i] && (s[*i] == ' ' || s[*i] == '\t'))
		(*i)++;
	return (*i);
}
//there are strange cases that need to be added(<>, ||)

int	syntax_error(const char *input)
{
	int		i;
	int		len;
	t_data	type;

	i = 0;
	while (input[i])
	{
		skip_spaces(input, &i);
		if (is_operator(&input[i], &len, &type))
		{
			i += len;
			skip_spaces(input, &i);
			if (is_operator(&input[i], &len, &type))
			{
				ft_putstr_fd("Syntax error near unexpected token `", 2);
				write(2, &input[i], len);
				write(2, "'\n", 2);
				return (1);
			}
			else if (!input[i])
			{
				write(2, "Syntax error near unexpected token `newline'\n", 45);
				return (1);
			}
		}
		else
			i++;
	}
	return (0);
}

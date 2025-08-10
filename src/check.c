#include "minishell.h"

int	has_open_quote(const char *s)
{
	int		i;
	char	q;

	i = 0;
	while (s && s[i])
	{
		if (s[i] == '\'' || s[i] == '"')
		{
			q = s[i];
			i++;
			while (s[i] && s[i] != q)
				i++;
			if (!s[i])
			{
				write(2, "\033[1;31mSyntax error: unclosed quote.\n", 38);
				return (1);
			}
		}
		if (s[i])
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
	int		k;
	t_data	type;

	i = 0;
	while (input && input[i])
	{
		skip_spaces(input, &i);
		if (!input[i])
			break ;
		if (is_operator(&input[i], &len, &type))
		{
			i += len;
			k = i;
			skip_spaces(input, &k);
			if (is_operator(&input[k], &len, &type))
			{
				write(2, "\033[1;31mSyntax error near unexpected token `", 44);
				write(2, &input[k], len);
				write(2, "'\n", 2);
				return (1);
			}
			if (!input[k])
			{
				write(2, "\033[1;31mSyntax error near unexpected token `newline'\n", 51);
				return (1);
			}
		}
		else
			i++;
	}
	return (0);
}

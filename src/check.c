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
				write(2, "shell: syntax error: unclosed quote.\n", 38);
				return (1);
			}
		}
		if (s[i])
			i++;
	}
	return (0);
}

static int	if_case(t_shell *sh, int i, int *len)
{
	t_data	type;

	if (((ft_strncmp(sh->tokens[i], ">", 1) == 0
				|| ft_strncmp(sh->tokens[i], ">>", 2) == 0)
			&& (is_operator(sh->tokens[i + 1], len, &type)
				|| !sh->tokens[i + 1]))
		|| (ft_strncmp(sh->tokens[i], "<", 1) == 0
			&& (is_operator(sh->tokens[i + 1], len, &type)
				|| !sh->tokens[i + 1]))
		|| (ft_strncmp(sh->tokens[i], "<<", 2) == 0
			&& (is_operator(sh->tokens[i + 1], len, &type)
				|| !sh->tokens[i + 1])))
		return (1);
	return (0);
}
static int	pr_error(t_shell *sh, int i, int *len)
{
	if (sh->tokens[i + 1] && ft_strncmp(sh->tokens[i + 1], " ", 2) == 0)
	{
		write(2, "shell: syntax error near unexpected token `newline'\n", 52);
		return (1);
	}
	write(2, "shell: syntax error near unexpected token `", 44);
	write(2, sh->tokens[i + 1], *len);
	write(2, "'\n", 2);
	return (1);
}

int	syntax_error(t_shell *sh)
{
	int		i;
	int		len;

	i = 0;
	while (sh->tokens[i])
	{
		if (i == 0 && (ft_strncmp(sh->tokens[i], "|", 1) == 0))
		{
			write(2, "shell: syntax error near unexpected token `|'\n", 46);
			return (1);
		}
		else if (if_case(sh, i, &len))
		{
			if (!sh->tokens[i + 1])
			{
				write(2, "shell: syntax error near ", 26);
				write(2, "unexpected token `newline'\n", 28);
				return (1);
			}
			else
				return (pr_error(sh, i, &len));
		}
		i++;
	}
	return (0);
}

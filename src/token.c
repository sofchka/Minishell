#include "minishell.h"

int	is_operator(const char *s, int *len, t_data *type)
{
	if (!s || !*s)
		return (0);

	if (s[0] == '|' && s[1] == '|')
		return (*len = 2, *type = PIPE, 1);
	if (s[0] == '>' && s[1] == '>')
		return (*len = 2, *type = APPEND, 1);
	if (s[0] == '<' && s[1] == '<')
		return (*len = 2, *type = HEREDOC, 1);
	if (s[0] == '|')
		return (*len = 1, *type = PIPE, 1);
	if (s[0] == '<')
		return (*len = 1, *type = REDIR_IN, 1);
	if (s[0] == '>')
		return (*len = 1, *type = REDIR_OUT, 1);
// if (!ft_strncmp(s, ">>", 2))
// 	return (*len = 2, *type = APPEND, 1);
// if (!ft_strncmp(s, "<<", 2))
// 	return (*len = 2, *type = HEREDOC, 1);
	return (0);
}

static int	count_tokens(const char *s)
{
	int		count;
	int		i;
	int		len;
	t_data	type;

	count = 0;
	i = 0;
	while (s[i])
	{
		while (s[i] == ' ' || s[i] == '\t')
			i++;
		if (!s[i])
			break ;
		if (is_operator(&s[i], &len, &type))
			i += len;
		else
		{
			while (s[i] && !is_operator(&s[i], &len, &type))
				i++;
		}
		count++;
	}
	return (count);
}

static char	*extract_token(const char *s, int *i)
{
	int		start;
	int		len;
	t_data	type;

	start = *i;
	len = 0;
	if (is_operator(&s[*i], &len, &type))
	{
		*i += len;
		return (ft_substr(s, start, len));
	}
	while (s[*i] && !is_operator(&s[*i], &len, &type))
		(*i)++;
	return (ft_substr(s, start, *i - start));
}

int	token(t_shell *sh, int i, int j)
{
	if (has_open_quote(sh->input) || syntax_error(sh->input))
		return (1);
	sh->tok_count = count_tokens(sh->input);
	if (sh->tok_count == 0)
		return (1);
	sh->tokens = malloc(sizeof(char *) * (sh->tok_count + 1));
	if (!sh->tokens)
		return (1);
	while (sh->input[i])
	{
		while (sh->input[i] == ' ' || sh->input[i] == '\t')
			i++;
		if (!sh->input[i])
			break ;
		sh->tokens[j] = extract_token(sh->input, &i);
		if (!sh->tokens[j])
			return (ft_free(sh->tokens), 1);
		if (ft_strncmp(sh->tokens[j], "|", 1) == 0)
			sh->pipe_count++;
		j++;
	}
	sh->tokens[j] = NULL;
	return (0);
}

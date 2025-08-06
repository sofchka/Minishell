#include "minishell.h"

static int	is_operator(const char *s, int *len, t_data *type)
{
	if (!ft_strncmp(s, ">>", 2))
		return (*len = 2, *type = APPEND, 1);
	if (!ft_strncmp(s, "<<", 2))
		return (*len = 2, *type = HEREDOC, 1);
	if (*s == '|')
		return (*len = 1, *type = PIPE, 1);
	if (*s == '<')
		return (*len = 1, *type = REDIR_IN, 1);
	if (*s == '>')
		return (*len = 1, *type = REDIR_OUT, 1);
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
	else
	{
		while (s[*i] && !is_operator(&s[*i], &len, &type))
			(*i)++;
		return (ft_substr(s, start, *i - start));
	}
}

int	token_loop(t_shell *sh, char **tokens, int i, int j)
{
	while (sh->input[i])
	{
		while (sh->input[i] == ' ' || sh->input[i] == '\t')
			i++;
		if (!sh->input[i])
			break ;
		tokens[j] = extract_token(sh->input, &i);
		if (!tokens[j])
			return (ft_free(tokens), 1);
		if (!ft_strncmp(tokens[j], "|", 1))
			sh->pipe_count++;
		else if (!ft_strncmp(tokens[j], "<", 1))
			sh->redir_in++;
		else if (!ft_strncmp(tokens[j], ">", 1))
			sh->redir_out++;
		else if (!ft_strncmp(tokens[j], ">>", 1))
			sh->append_out++;
		else if (!ft_strncmp(tokens[j], "<<", 1))
			sh->heredoc++;
		j++;
	}
	tokens[j] = NULL;
	return (0);
}

int	token(t_shell *sh)
{
	char	**tokens;

	init_2(sh);
	if (has_open_quote(sh->input))
	{
		write(2, "Syntax error: unclosed quote.\n", 31);
		return (1);
	}
	sh->tok_count = count_tokens(sh->input);
	if (sh->tok_count == 0)
		return (1);
	tokens = malloc(sizeof(char *) * (sh->tok_count + 1));
	if (!tokens)
		return (1);
	if (token_loop(sh, tokens, 0, 0))
		return (1);
	sh->tokens = tokens;
	return (0);
}

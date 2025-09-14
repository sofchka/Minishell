#include "minishell.h"

static int	skip_quotes(const char *s, int *i)
{
	char	quote;

	quote = s[*i];
	(*i)++;
	while (s[*i] && s[*i] != quote)
		(*i)++;
	if (s[*i] == quote)
		(*i)++;
	else
		return (1);
	return (0);
}

// static int	count_tokens(const char *s, int count)
// {
// 	int		len;
// 	t_data	type;
// 	int		i;

// 	i = 0;
// 	while (s[i])
// 	{
// 		while (s[i] == ' ' || s[i] == '\t')
// 			i++;
// 		if (!s[i])
// 			break ;
// 		if (is_operator(&s[i], &len, &type))
// 		{
// 			// normal operator
// 			count++;

// 			// heredoc or input redirection: might need placeholder '-'
// 			if ((ft_strncmp(&s[i], "<<", 2) == 0)
// 				|| (ft_strncmp(&s[i], "<", 1) == 0))
// 				count++;  // reserve space for the "-"
// 			i += len;
// 		}
// 		else
// 		{
// 			while (s[i] && !is_operator(&s[i], &len, &type))
// 			{
// 				if (s[i] == '"' || s[i] == '\'')
// 					skip_quotes(s, &i);
// 				i++;
// 			}
// 			count++;
// 		}
// 	}
// 	return (count);
// }

static int	count_tokens(const char *s, int count)
{
	int		len;
	t_data	type;
	int		i;

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
			{
				if (s[i] == '"' || s[i] == '\'')
					skip_quotes(s, &i);
				i++;
			}
		}
		count++;
	}
	return (count);
}

static char	*extract_token(const char *s, int *i, int start, int end)
{
	char	quote;
	int		len;
	t_data	type;

	if (is_operator(&s[*i], &len, &type))
	{
		*i += len;
		return (ft_substr(s, start, len));
	}
	while (s[*i] && !is_operator(&s[*i], &len, &type))
	{
		if (s[*i] == '"' || s[*i] == '\'')
		{
			quote = s[(*i)++];
			while (s[*i] && s[*i] != quote)
				(*i)++;
			if (s[*i] == quote)
				(*i)++;
		}
		(*i)++;
	}
	end = *i;
	while (end > start && (s[end - 1] == ' ' || s[end - 1] == '\t'))
		end--;
	return (ft_substr(s, start, end - start));
}

static int	token_loop(t_shell *sh, int i, int j)
{
	while (sh->input[i])
	{
		while (sh->input[i] == ' ' || sh->input[i] == '\t')
			i++;
		if (!sh->input[i])
			break ;
		sh->tokens[j] = extract_token(sh->input, &i, i, 0);
		if (!sh->tokens[j])
			return (ft_free(sh->tokens), 1);
		if (ft_strncmp(sh->tokens[j], "|", 1) == 0)
			sh->pipe_count++;
		if (ft_strncmp(sh->tokens[j], "<<", 2) == 0)
			sh->heredocs++;
		if ((ft_strncmp(sh->tokens[j], "<<", 2) == 0
				|| ft_strncmp(sh->tokens[j], "<", 1) == 0)
			&& ((j > 0 && ft_strncmp(sh->tokens[j - 1], "|", 1)) == 0
				|| j == 0))
		{
			sh->tokens[j + 1] = sh->tokens[j];
			sh->tokens[j++] = ft_strdup("-");
		}
		j++;
	}
	sh->tokens[j] = NULL;
	return (0);
}

int	token(t_shell *sh, int i, int j)
{
	if (has_open_quote(sh->input))
		return (1);
	sh->tok_count = count_tokens(sh->input, 0);
	if (sh->tok_count == 0)
		return (1);
	sh->tokens = malloc(sizeof(char *) * (sh->tok_count + 3));
	if (!sh->tokens)
		return (1);
	if (token_loop(sh, i, j))
		return (1);
	if (syntax_error(sh))
	{
		ft_free(sh->tokens);
		return (1);
	}
	if (sh->tok_count == 1 && sh->tokens[0])
	{
		if (sh->tokens[0] && ft_strncmp(sh->tokens[0], "exit", 4) == 0)
			return (ft_exit(ft_split(sh->tokens[0], ' '), sh), 1);
	}
	return (0);
}

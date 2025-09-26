#include "minishell.h"

static int	count_tokens(const char *s, int count)
{
	int		i;
	int		len;
	t_data	type;
	int		expect_cmd;
	int		need_rarg;

	i = 0;
	expect_cmd = 1;
	need_rarg = 0;
	while (s[i])
	{
		while (s[i] == ' ' || s[i] == '\t')
			i++;
		if (!s[i])
			break ;
		if (is_operator(&s[i], &len, &type))
		{
			count++;
			if (expect_cmd && (type == HEREDOC || type == REDIR_IN))
				count++;
			if (type == HEREDOC || type == REDIR_IN
				|| type == REDIR_OUT || type == APPEND)
				need_rarg = 1;
			if (type == PIPE)
				expect_cmd = 1;
			i += len;
		}
		else
		{
			while (s[i] && !is_operator(&s[i], &len, &type))
			{
				if (s[i] == '"' || s[i] == '\'')
				{
					char	 q;
					q = s[i++];
					while (s[i] && s[i] != q)
						i++;
					if (s[i] == q)
						i++;
				}
				else
					i++;
			}
			count++;
			if (need_rarg)
				need_rarg = 0;
			else
				expect_cmd = 0;
		}
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
			&& ((j > 0 && (ft_strncmp(sh->tokens[j - 1], "|", 1) == 0
						|| (ft_isalnum(sh->tokens[j - 1][0]) //op chi kpoxem
			&& (j > 1 && (ft_strncmp(sh->tokens[j - 2], "<<", 2) == 0
				|| ft_strncmp(sh->tokens[j - 2], ">>", 2) == 0
				|| ft_strncmp(sh->tokens[j - 2], ">", 1) == 0
				|| ft_strncmp(sh->tokens[j - 2], "<", 1) == 0))))) || j == 0))
		{
			sh->tokens[j + 1] = sh->tokens[j];
			sh->tokens[j++] = ft_strdup(" ");
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
	sh->tokens = malloc(sizeof(char *) * (sh->tok_count + 1));
	if (!sh->tokens)
		return (1);
	if (token_loop(sh, i, j))
		return (1);
	if (syntax_error(sh))
	{
		g_exit_status = 2;
		ft_free(sh->tokens);
		return (1);
	}
	return (0);
}

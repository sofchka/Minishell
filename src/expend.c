#include "minishell.h"

static char	*get_env_value(const char *name, t_shell *shell)
{
	int		i;
	size_t	len;

	len = ft_strlen(name);
	if (ft_strncmp(name, "?", len + 1) == 0)
		return (ft_itoa(g_exit_status));
	i = 0;
	while (shell->env[i])
	{
		if (ft_strncmp(shell->env[i], name, len) == 0
			&& shell->env[i][len] == '=')
			return (ft_strdup(shell->env[i] + len + 1));
		i++;
	}
	return (ft_strdup(""));
}

static char	*expand_loop(char *input, t_shell *shell, char *result, int *i, int *j)
{
	char	quote;
	int		start;
	char	*var;
	char	*value;

	quote = 0;
	while (input[*i])
	{
		if (input[*i] == '\'' && !quote)
			quote = '\'';
		else if (input[*i] == quote)
			quote = 0;
		else if (input[*i] == '"' && !quote)
			quote = '"';
		else if (input[*i] == '$' && input[*i + 1]
			&& input[*i + 1] != ' ' && quote != '\'')
		{
			(*i)++;
			start = *i;
			while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '?'))
				(*i)++;
			var = ft_substr(input, start, *i - start);
			value = get_env_value(var, shell);
			ft_strlcat(result, value, ft_strlen(result) + ft_strlen(value) + 1);
			*j += ft_strlen(value);
			free(var);
			free(value);
			continue ;
		}
		result[(*j)++] = input[*i];
		(*i)++;
	}
	result[*j] = '\0';
	return (result);
}

char	*expand_vars(char *input, t_shell *shell)
{
	char	*result;
	int		i;
	int		j;

	result = ft_calloc(1, ft_strlen(input) * 2 + 1);
	i = 0;
	j = 0;
	result = expand_loop(input, shell, result, &i, &j);
	return (result);
}

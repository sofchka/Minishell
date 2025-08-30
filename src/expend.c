#include "minishell.h"

void	*ft_realloc(void *ptr, size_t size)
{
	void	*new_ptr;

	if (!ptr)
		return (ft_calloc(1, size));
	if (!size)
	{
		free(ptr);
		return (NULL);
	}
	new_ptr = ft_calloc(1, size);
	if (!new_ptr)
		return (NULL);
	ft_memcpy(new_ptr, ptr, size);
	free(ptr);
	return (new_ptr);
}

static char	*get_env_value(char *name, t_shell *shell, int start)
{
	size_t	len;
	int		i;

	if (!name || !shell || !shell->env)
		return (ft_strdup(""));
	len = ft_strlen(name);
	i = 0;
	if (ft_strncmp(name, "?", 1) == 0)
	{
		while (start > 0 && shell->input[start] == ' ')
			start--;
		if (start > 0 && ((shell->input[start] == '<'
					&& shell->input[start - 1] == '<')))
			return (ft_strjoin("$", name, 0));
		return (ft_strjoin(ft_itoa(g_exit_status), &name[1], 0));
	}
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

static char	*expand_loop(char *input, t_shell *shell, char *result, size_t *i, size_t *j, size_t result_size)
{
	char	quote;
	size_t	start;
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
		else if (input[*i] == '$' && input[*i + 1] && input[*i + 1] != ' '
			&& input[*i + 1] != '"' && input[*i + 1] != '\'' && quote != '\'')
		{
			(*i)++;
			start = *i;
			while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '?'))
				(*i)++;
			var = ft_substr(input, start, *i - start);
			if (!var)
				return (result);
			value = get_env_value(var, shell, start - 2);
			if (!value)
			{
				free(var);
				return (result);
			}
			if (*j + ft_strlen(value) >= result_size)
			{
				result_size = result_size * 2 + ft_strlen(value) + 1;
				result = ft_realloc(result, result_size);
				if (!result)
				{
					free(var);
					free(value);
					return (NULL);
				}
			}
			ft_strlcat(result, value, result_size);
			*j += ft_strlen(value);
			free(var);
			free(value);
			continue ;
		}
		if (*j + 1 >= result_size)
		{
			result_size = result_size * 2 + 1;
			result = ft_realloc(result, result_size);
			if (!result)
				return (NULL);
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
	size_t	i;
	size_t	j;
	size_t	result_size;

	j = 0;
	i = 0;
	if (!input || !shell)
		return (ft_strdup(""));

	result_size = ft_strlen(input) * 2 + 1;
	result = ft_calloc(1, result_size);
	if (!result)
		return (NULL);

	result = expand_loop(input, shell, result, &i, &j, result_size);
	return (result);
}
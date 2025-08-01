#include "minishell.h"

static int	len_input(char *str)
{
	int	len;
	int	i;

	i = 0;
	len = 0;
	while (str[i] == ' ' || str[i] == '\t')
		i++;
	while (str[i])
	{
		if (str[i] == ' ' || str[i] == '\t')
		{
			len++;
			while (str[i] == ' ' || str[i] == '\t')
				i++;
		}
		else
		{
			len++;
			i++;
		}
	}
	return (len);
}

char	*ft_space(char *str, int j)
{
	char	*res;
	int		i;

	res = malloc(j + 1);
	if (!res)
		return (NULL);
	i = 0;
	while (str[i] == ' ' || str[i] == '\t')
		i++;
	j = 0;
	while (str[i])
	{
		if (str[i] == ' ' || str[i] == '\t')
		{
			res[j++] = ' ';
			while (str[i] == ' ' || str[i] == '\t')
				i++;
		}
		else
			res[j++] = str[i++];
	}
	if (j > 0 && res[j - 1] == ' ')
		j--;
	res[j] = '\0';
	return (res);
}

int	minishell_start(t_shell *shell)
{
	char	*clean_input;

	while (1)
	{
		shell->input = readline("\033[1;34mMiniShell $ \033[1;36m");
		if (!shell->input)
			break ;
		if (shell->input && *shell->input)
			add_history(shell->input);
		clean_input = ft_space(shell->input, len_input(shell->input));
		free(shell->input);
		shell->input = clean_input;
		if (!shell->input)
			break ;
		// commands 
		// parse + exec ?
		if (ft_strncmp(shell->input, "exit", 4) == 0)
		{
			free(shell->input);
			write(1, "exit\n", 5);
			break ;
		}
		free(shell->input);
	}
	return (0);
}

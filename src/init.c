#include "minishell.h"

char	**dup_env(char **envp)
{
	char	**res;
	int		i;

	i = 0;
	while (envp[i])
		i++;
	res = (char **)malloc(sizeof(char *) * (i + 1));
	if (!res)
		return (NULL);
	i = -1;
	while (envp[++i])
	{
		res[i] = ft_strdup(envp[i]);
		if (!res[i])
		{
			while (i-- > 0)
				free(res[i]);
			free(res);
			return (NULL);
		}
	}
	res[i] = NULL;
	return (res);
}

void	init_shell(t_shell *shell)
{
	shell->input = NULL;
	shell->paths = NULL;
	shell->stdin_backup = dup(STDIN_FILENO);
	shell->stdout_backup = dup(STDOUT_FILENO);
//...
}

void	init_2(t_shell *sh)
{
	sh->pipe_count = 0;
	sh->redir_in = 0;
	sh->redir_out = 0;
	sh->append_out = 0;
	sh->heredoc = 0;
}

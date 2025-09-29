#include "minishell.h"

char	**dup_env(char **envp)
{
	char	**res;
	int		i;

	i = 0;
	while (envp && envp[i])
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
	shell->t_env = NULL;
	shell->input = NULL;
	shell->tokens = NULL;
	shell->home = NULL;
	shell->tok_count = 0;
	shell->pipe_count = 0;
	shell->env = NULL;
	shell->cmds = NULL;
	shell->heredocs = 0;
	shell->redirs = 0;
	shell->stdin_backup = dup(STDIN_FILENO);
	shell->stdout_backup = dup(STDOUT_FILENO);
}

void	reinit(t_shell **shell)
{
	(*shell)->tok_count = 0;
	(*shell)->pipe_count = 0;
	(*shell)->heredocs = 0;
	(*shell)->redirs = 0;
	free((*shell)->input);
	ft_free((*shell)->tokens);
	restore_std(*shell);
}

void init_env(t_shell *shell,char **envp)
{
	t_env* tail = NULL;
	t_env* new_node;
	int i = -1;
	while(envp[++i])
	{
		new_node = malloc(sizeof(t_env));
		if(!new_node)
		{
			printf("error");
		}
		new_node->key = ft_substr(envp[i], 0, ft_strchr(envp[i],'=') - envp[i]);
		new_node->value = ft_strdup(ft_strchr(envp[i],'=') + 1);
		new_node->next = NULL;
		if(shell->t_env == NULL)
			shell->t_env = new_node;
		else
			tail->next = new_node;
		tail = new_node;
	}
} 

#include "../minishell.h"

void	unset_env(t_shell *sh, char **cmd, int count)
{
	t_env	*tmp;
	t_env	*prev;

	tmp = sh->t_env;
	prev = NULL;
	while (tmp != NULL)
	{
		if (!ft_strcmp(tmp->key, cmd[count]))
		{
			if (prev == NULL)
				sh->t_env = tmp->next;
			else
				prev->next = tmp->next;
			free(tmp->key);
			free(tmp->value);
			free(tmp);
			return ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}

int	ft_unset(t_shell *sh, char **cmd)
{
	if (!sh->t_env)
		return (0);
	int count = cmd_count(cmd);
	while (count-- != 1)
	{
		unset_env(sh, cmd, count);
	}
	return (0);
}
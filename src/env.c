#include "minishell.h"

int	ft_env(t_shell *sh, char **cmd)
{
	t_env	*tmp;

	tmp = sh->t_env;
	if (cmd[1])
	{
		printf("env: '%s': No such file or directory\n", cmd[1]);
		return (g_exit_status = 127, 1);
	}
	while (tmp)
	{
		if (tmp->key && tmp->value)
			printf("%s=%s\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}
	return (0);
}

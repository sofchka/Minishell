#include "minishell.h"

int	valid_unset_key(char *str)
{
	int	i;

	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);
	i = 1;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

void	unset_env(t_shell *sh, char *key)
{
	t_env	*tmp;
	t_env	*prev;

	prev = NULL;
	tmp = sh->t_env;
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, key))
		{
			if (prev)
				prev->next = tmp->next;
			else
				sh->t_env = tmp->next;
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
	int	i;
	int	status;

	if (!cmd || !cmd[1])
		return (g_exit_status = 0, 0);
	status = 0;
	i = 1;
	while (cmd[i])
	{
		if (!valid_unset_key(cmd[i]))
		{
			ft_putstr_fd("bash: unset: `", 2);
			ft_putstr_fd(cmd[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			status = 1;
		}
		else
			unset_env(sh, cmd[i]);
		i++;
	}
	g_exit_status = status;
	return (status);
}

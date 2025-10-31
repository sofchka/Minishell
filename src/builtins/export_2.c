#include "../minishell.h"

static int	valid_export_key(char *s)
{
	int	i;

	if (!s || (!ft_isalpha((unsigned char)s[0]) && s[0] != '_'))
		return (0);
	i = 1;
	while (s[i] && s[i] != '=')
	{
		if (!ft_isalnum((unsigned char)s[i]) && s[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static int	export_one(t_shell *sh, char *arg)
{
	char	*eq;
	char	*key;
	char	*val;
	t_env	*n;

	if (!valid_export_key(arg))
	{
		ft_putstr_fd("bash: export: `", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		return (1);
	}
	eq = ft_strchr(arg, '=');
	val = NULL;
	if (eq)
	{
		key = ft_substr(arg, 0, (size_t)(eq - arg));
		val = ft_strdup(eq + 1);
	}
	else
		key = ft_strdup(arg);
	if (!key || (eq && !val))
		return (free(key), free(val), 1);
	n = sh->t_env;
	while (n)
	{
		if (!ft_strcmp(n->key, key))
		{
			if (eq)
			{
				free(n->value);
				n->value = val;
			}
			else
				free(val);
			return (free(key), 0);
		}
		n = n->next;
	}
	n = malloc(sizeof(t_env));
	if (!n)
		return (free(key), free(val), 1);
	n->key = key;
	n->value = val;
	n->next = sh->t_env;
	sh->t_env = n;
	return (0);
}

int	ft_export(t_shell *sh, char **cmd)
{
	int	i;
	int	err;

	if (cmd_count(cmd) == 1)
		return (ft_print_export(sh), ft_free(cmd), g_exit_status = 0, 0);
	err = 0;
	i = 1;
	while (cmd[i])
	{
		if (export_one(sh, cmd[i]))
			err = 1;
		i++;
	}
	g_exit_status = err;
	return (ft_free(cmd), err);
}

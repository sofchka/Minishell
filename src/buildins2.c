#include "minishell.h"

static void export_print(t_shell *shell)
{
	int i;

	i = 0;
	while (shell->env[i])
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putstr_fd(shell->env[i], STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
		i++;
	}
}

static char	*get_var_name(char *arg)
{
	int		i;
	char	*name;

	i = 0;
	while (arg[i] && arg[i] != '=')
		i++;
	name = ft_substr(arg, 0, i);
	return (name);
}

static int	export_add(char *arg, t_shell *shell)
{
	char	*name;
	char	*value;
	char	*new_env;
	int	j;

	name = get_var_name(arg);
	value = ft_strchr(arg, '=') ? ft_strchr(arg, '=') + 1 : "";
	if (!name || !ft_isalpha(name[0]))
	{
		ft_putstr_fd("export: `", STDERR_FILENO);
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
		return (1);
	}
	new_env = ft_strjoin(name, "=", 0);
	new_env = ft_strjoin(new_env, value, 1);
	j = 0;
	while (shell->env[j] && ft_strncmp(shell->env[j], name, ft_strlen(name)))
		j++;
	if (shell->env[j])
	{
		free(shell->env[j]);
		shell->env[j] = new_env;
	}
	else
	{
		char	**new_envp;
		int		k;

		new_envp = malloc(sizeof(char *) * (j + 2));
		k = 0;
		while (k < j)
		{
			new_envp[k] = shell->env[k];
			k++;
		}
		new_envp[j] = new_env;
		new_envp[j + 1] = NULL;
		free(shell->env);
		shell->env = new_envp;
	}
	return (0);
}

int	ft_export(char **argv, t_shell *shell)
{
	int	i;
	int	status;

	i = 1;
	status = 0;
	if (!argv[1])
	{
		export_print(shell);
		return (0);
	}
	while (argv[i])
	{
		if (export_add(argv[i], shell))
			status = 1;
		i++;
	}
	return (status);
}

int	ft_unset(char **argv, t_shell *shell)
{
	int	i;
	int	j;
	int	status;

	i = 1;
	status = 0;
	while (argv[i])
	{
		if (!ft_isalpha(argv[i][0]))
		{
			ft_putstr_fd("unset: `", STDERR_FILENO);
			ft_putstr_fd(argv[i], STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			status = 1;
		}
		else
		{
			j = 0;
			while (shell->env[j] && ft_strncmp(shell->env[j], argv[i], ft_strlen(argv[i])))
				j++;
			if (shell->env[j] && ft_strchr(shell->env[j], '='))
			{
				free(shell->env[j]);
				while (shell->env[j])
				{
					shell->env[j] = shell->env[j + 1];
					j++;
				}
			}
		}
		i++;
	}
	return (status);
}
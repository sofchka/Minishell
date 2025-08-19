#include "minishell.h"

int	ft_echo(char *arg)
{
	int		i;
	int		no_newline;
	char	**argv;

	argv = ft_split(arg, ' ');
	i = 1;
	no_newline = 0;
	if (argv[i] && ft_strncmp(argv[i], "-n", ft_strlen(argv[i]) + 1) == 0)
	{
		no_newline = 1;
		i++;
	}
	while (argv[i])
	{
		ft_putstr_fd(argv[i], STDOUT_FILENO);
		if (argv[i + 1])
			ft_putstr_fd(" ", STDOUT_FILENO);
		i++;
	}
	if (!no_newline)
		ft_putstr_fd("\n", STDOUT_FILENO);
	else
		ft_putstr_fd("%%", STDOUT_FILENO);
	return (0);
}

int	ft_cd(char **argv, t_shell *shell)
{
	char	*path;
	char	*oldpwd;
	char	cwd[1024];

	if (argv[1] && argv[2])
	{
		ft_putstr_fd("cd: too many arguments\n", STDERR_FILENO);
		return (1);
	}
	path = argv[1] ? argv[1] : getenv("HOME");
	if (!path)
	{
		ft_putstr_fd("cd: HOME not set\n", STDERR_FILENO);
		return (1);
	}
	oldpwd = getcwd(cwd, sizeof(cwd));
	if (chdir(path) != 0)
	{
		perror("cd");
		return (1);
	}
	if (oldpwd)
	{
		ft_export((char *[]){"export", ft_strjoin("OLDPWD=", oldpwd, 0), NULL}, shell);
		path = getcwd(cwd, sizeof(cwd));
		if (path)
			ft_export((char *[]){"export", ft_strjoin("PWD=", path, 0), NULL}, shell);
	}
	return (0);
}

int ft_pwd(void)
{
	char cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("pwd");
		return (1);
	}
	ft_putstr_fd(cwd, STDOUT_FILENO);
	ft_putstr_fd("\n", STDOUT_FILENO);
	return (0);
}

int ft_env(t_shell *shell)
{
	int i;

	i = 0;
	while (shell->env[i])
	{
		if (ft_strchr(shell->env[i], '='))
		{
			ft_putstr_fd(shell->env[i], STDOUT_FILENO);
			ft_putstr_fd("\n", STDOUT_FILENO);
		}
		i++;
	}
	return (0);
}

int	ft_exit(char **argv, t_shell *shell)
{
	int	status;

	status = g_exit_status;
	ft_putstr_fd("exit\n", STDOUT_FILENO);
	if (argv[1] && argv[2])
	{
		ft_putstr_fd("exit: too many arguments\n", STDERR_FILENO);
		return (1);
	}
	if (argv[1])
	{
		if (!ft_isdigit(argv[1][0]))
		{
			ft_putstr_fd("exit: ", STDERR_FILENO);
			ft_putstr_fd(argv[1], STDERR_FILENO);
			ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
			status = 2;
		}
		else
			status = ft_atoi(argv[1]);
	}
	ft_free(shell->env);
	free(shell->input);
	ft_free(shell->tokens);
	close(shell->stdin_backup);
	close(shell->stdout_backup);
	exit(status);
}

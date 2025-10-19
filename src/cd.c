#include "minishell.h"

int	cmd_count(char **cmd)
{
	int	i;

	i = 0;
	while (cmd[i] && cmd)
		i++;
	return (i);
}

int	ft_cd(t_shell *sh, char **cmd)
{
	char	*path;
	int		ccmd;
	char	*a;
	char	l[1024];
	char	*verj;
	char	**pwd;

	ccmd = cmd_count(cmd);
	if (ccmd > 2)
		return (write(2, "bash: cd: too many arguments\n", 29),
			g_exit_status = 1, 1);

	if (ccmd == 1)
	{
		path = get_env_value("HOME", sh);
		if (!path || path[0] == '\0')
			return (write(2, "bash: cd: HOME not set\n", 23), free(path),
				g_exit_status = 1, 1);
		if (chdir(path) == -1)
			return (perror("cd"), free(path), g_exit_status = 1, 1);
		free(path);
	}
	else if (ccmd == 2 && !ft_strcmp(cmd[1], "~"))
	{
		if (chdir(sh->home) == -1)
			return (perror("cd"), g_exit_status = 1, 1);
	}
	else if (chdir(cmd[1]) == -1)
	{
		a = ft_strjoin("cd: ", cmd[1], 0);
		if (!a)
			return (g_exit_status = 1, 1);
		perror(a);
		free(a);
		return (g_exit_status = 1, 1);
	}

	
	if (!getcwd(l, sizeof(l)))
		return (perror("cd: getcwd"), g_exit_status = 1, 1);

	verj = ft_strjoin("PWD=", l, 0);
	pwd = malloc(sizeof(char *) * 3);
	if (!pwd || !verj)
		return (free(verj), g_exit_status = 1, 1);

	pwd[0] = ft_strdup("export");
	pwd[1] = ft_strdup(verj);
	pwd[2] = NULL;

	free(verj);
	ft_export(sh, pwd);
	ft_free(pwd);

	return (g_exit_status = 0, 0);
}

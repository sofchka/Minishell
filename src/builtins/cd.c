#include "../minishell.h"

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
	char	oldpwd[1024];
	char	**export_dot;
	char	*pwd_value;
	char *s = NULL;

	ccmd = cmd_count(cmd);
	export_dot = malloc(3 * sizeof(char *));
	export_dot[0] = "export";
	export_dot[2] = NULL;
	if (ccmd > 2)
		return (write(2, "bash: cd: too many arguments\n", 29),
			g_exit_status = 1, 1);
	if (!getcwd(oldpwd, sizeof(oldpwd)))
	{
		pwd_value = ft_strjoin("OLDPWD=", get_env_value("PWD", sh), 0);
		export_dot[1] = pwd_value;
		ft_export(sh, export_dot);
		printf("cd: error retrieving current directory: getcwd: cannot access parent directories:\n");
	}
	else
	{
		s =  ft_strjoin("OLDPWD=",getcwd(NULL,0),0);
		export_dot[1] = s;
		ft_export(sh,export_dot);
		free(s);
	}
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
	if (!getcwd(oldpwd, sizeof(oldpwd)))
	{
		pwd_value = ft_strjoin("PWD=", get_env_value("PWD", sh), 0);
		export_dot[1] = ft_strjoin(pwd_value, "/..", 0);
		ft_export(sh, export_dot);
	}
	else
	{
		s = ft_strjoin("PWD=",getcwd(NULL,0),0);
		export_dot[1] = s;
		ft_export(sh,export_dot);
		free(s);
	}
	if (g_exit_status == 1)
		return (1);
	return (0);
}

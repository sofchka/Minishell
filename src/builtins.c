#include "minishell.h"

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (/*!ft_strcmp(cmd, "echo") || */!ft_strcmp(cmd, "cd")
		|| !ft_strcmp(cmd, "pwd") /*|| !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "env")*/
		|| !ft_strcmp(cmd, "exit"));
}

int	exec_builtin(t_shell *sh, char **cmd, t_exec *cmds)
{
	if (handle_redirection(cmds) == -1)
	{
		restore_std(sh);
		return (1);
	}
	// cd export unset exit aranc fork
	if (!cmd)
		return (1);
	/*if (!ft_strcmp(cmd->cmd, "echo"))
		return (ft_echo(&cmd->cmd));*/
	else if (!ft_strcmp(cmd[0], "cd"))
		return (ft_cd(sh, cmd));
	else if (!ft_strcmp(cmd[0], "pwd"))
		return (ft_pwd());
	/*else if (!ft_strcmp(cmd->cmd, "export"))
		return (ft_export(sh, &cmd->cmd));
	else if (!ft_strcmp(cmd->cmd, "unset"))
		return (ft_unset(sh, &cmd->cmd));
	else if (!ft_strcmp(cmd->cmd, "env"))
		return (ft_env(sh->env));*/
	else if (!ft_strcmp(cmd[0], "exit"))
		return (ft_exit(cmd, sh), 1);
	restore_std(sh);
	ft_free(cmd);
	return (1);
}

int	ft_pwd(void)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		ft_exit_perror("pwd");
		return (1);
	}
	write(STDOUT_FILENO, pwd, ft_strlen(pwd));
	write(STDOUT_FILENO, "\n", 1);
	free(pwd);
	return (0);
}
//cd

int cmd_count(char **cmd)
{
	int i = 0;
	while(cmd[i])
		i++;
	return i;
}

char	*env_value(char *name, t_shell *sh)
{
	int		i;
	size_t	len;

	if (!name || !sh || !sh->env)
		return (NULL);
	len = ft_strlen(name);
	i = 0;
	while (sh->env[i])
	{
		if (!ft_strncmp(sh->env[i], name, len) && sh->env[i][len] == '=')
			return (ft_strdup(sh->env[i] + len + 1));
		i++;
	}
	return (NULL);
}

int ft_cd(t_shell *sh, char **cmd)
{
	char	*path;
	int		i;
	int		ccmd;

	ccmd = cmd_count(cmd);
	if (ccmd > 2)
		return (write(2, "bash: cd: too many arguments\n", 30), g_exit_status = 1, 1);
	i = -1;
	if (ccmd == 1) // only "cd"
	{
		while (ccmd == 1 && sh->env[++i])
		{
			path = env_value("HOME", sh);
			if (path == NULL)
				return (write(2, "bash: cd: HOME not set\n", 23), g_exit_status = 1, 1);
			if (chdir(path) == -1)
				return (ft_exit_perror("cd"), free(path), g_exit_status = 1, 1);
			free(path);
		}
	}
	else
	{
		if (chdir(cmd[1]) == -1)
			return (ft_exit_perror("cd"), g_exit_status = 1, 1);
	}
	return (g_exit_status = 0, 0);
}

#include "minishell.h"

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd")
		|| !ft_strcmp(cmd, "pwd") || !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "env")
		|| !ft_strcmp(cmd, "exit"));
}

int	exec_builtin(t_shell *sh, char **cmd, t_exec *cmds)
{
	if (handle_redirection(cmds) == -1)
	{
		restore_std(sh);
		return (1);
	}
	if (!cmd)
		return (1);
	if (!ft_strcmp(cmd[0], "echo"))
		return (ft_echo(sh, cmd));
	else if (!ft_strcmp(cmd[0], "cd"))
		return (ft_cd(sh, cmd));
	else if (!ft_strcmp(cmd[0], "pwd"))
		return (ft_pwd(sh));
	else if (!ft_strcmp(cmd[0], "export"))
		return (ft_export(sh, cmd));
	else if (!ft_strcmp(cmd[0], "unset"))
		return (ft_unset(sh, cmd));
	else if (!ft_strcmp(cmd[0], "env"))
		return (ft_env(sh, cmd));
	else if (!ft_strcmp(cmd[0], "exit"))
		return (ft_exit(cmd, sh), 1);
	restore_std(sh);
	ft_free(cmd);
	return (1);
}

#include "minishell.h"

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (/*!ft_strcmp(cmd, "echo") || */!ft_strcmp(cmd, "cd")
		|| !ft_strcmp(cmd, "pwd") /*|| !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "env")
		|| !ft_strcmp(cmd, "exit")*/);
}

int	exec_builtin(t_shell *sh, char **cmd)
{
    (void)sh;
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
		return (ft_env(sh->env));
	else if (!ft_strcmp(cmd->cmd, "exit"))
		return (ft_exit(sh, &cmd->cmd));*/
    ft_free(cmd);
	return (1);
}

int	ft_pwd(void)
{
    char *pwd;
    pwd = getcwd(NULL, 0);
    if (!pwd)
    {
		perror("pwd");
		return 1;
    }
	printf("%s\n",pwd);
	free(pwd);
    return 0;
}
//cd

int cmd_count(char **cmd)
{
	int i = 0;
	while(cmd[i])
		i++;
	return i;
}

int ft_cd(t_shell *sh, char **cmd)
{
	int cd;
	char *path;
	//printf("lalalla");
	//printf("%s",cmd[1]);
	int ccmd = cmd_count(cmd);
	if(ccmd > 2)
		return (printf("bash: cd: too many arguments\n"),1);
	int i = -1;
	while(ccmd == 1 && sh->env[++i])
	{
		if((path = ft_strnstr(sh->env[i],"HOME=",5)) != NULL)
		{
			cd = chdir(path + 5);
			return 0;
		}
		//printf("%s\n",sh->env[i]);
	}
	cd = chdir(cmd[1]);
	return 0;
}


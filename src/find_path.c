#include "minishell.h"

void	ft(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}

int	find_path(char **envp, char ***path)
{
	while (*envp && ft_strncmp("PATH=", *envp, 5))
		envp++;
	if (!*envp)
		return (0);
	*path = ft_split(*envp + 5, ':');
	if (!*path)
		return (0);
	return (1);
}

char	*full_cmd(char **cmd)
{
	if (access(cmd[0], X_OK) == 0)
		return (ft_strdup(cmd[0]));
	return (ft(cmd), NULL);
}

char	*find_cmd(char *command, char **envp, int i, char *tmp)
{
	char	**paths;
	char	**cmd;

	if (find_path(envp, &paths) == 0)
		return (NULL);
	cmd = ft_split(command, ' ');
	if (!cmd || !cmd[0])
		return (ft(paths), NULL);
	if (cmd[0][0] == '/')
		return (ft(paths), full_cmd(cmd));
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/", 0);
		if (!tmp)
			return (ft(paths), ft(cmd), NULL);
		tmp = ft_strjoin(tmp, cmd[0], 1);
		if (!tmp)
			return (ft(paths), ft(cmd), NULL);
		if (access(tmp, X_OK) == 0)
			return (ft(paths), ft(cmd), tmp);
		free(tmp);
		i++;
	}
	return (ft(paths), ft(cmd), NULL);
}

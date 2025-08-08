#include "minishell.h"

void	ft_exit(char *err) //!
{
	perror(err);
	exit(1);
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

char	*find_cmd(char *command, char **envp, int i, char *tmp)
{
	char	**paths;
	char	**cmd;

	if (find_path(envp, &paths) == 0)
		return (NULL);
	while (paths[i])
	{
		cmd = ft_split(command, ' ');
		if (!cmd || !cmd[0])
			return (ft(paths, NULL), NULL);
		tmp = ft_strjoin(paths[i], "/", 0);
		if (!tmp)
			return (ft(paths, NULL), ft(cmd, NULL), NULL);
		if (cmd[0][0] != '/')
			tmp = ft_strjoin(tmp, cmd[0], 1);
		else
			tmp = ft_strjoin(tmp, strrchr(*cmd, '/') + 1, 1);
		if (!tmp)
			return (ft(paths, NULL), ft(cmd, NULL), NULL);
		if (access(tmp, X_OK) == 0)
			return (ft(paths, NULL), ft(cmd, NULL), tmp);
		ft(cmd, tmp);
		i++;
	}
	return (ft(paths, NULL), NULL);
}

void	ft(char **str, char *arr)
{
	int	i;

	if (arr)
		free(arr);
	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}

int	start(t_shell *sh)
{
	t_vars	var;
	int		i;
	int		j;
	int		cmd_count;

	cmd_count = sh->pipe_count + 1;
	var.pfd = malloc(sizeof(int) * 2 * sh->pipe_count);
	var.pids = malloc(sizeof(pid_t) * cmd_count);
	if (!var.pfd || !var.pids)
		ft_exit("malloc failed\n");
	i = -1;
	while (++i < sh->pipe_count)
	{
		if (pipe(var.pfd + i * 2) == -1)
			ft_exit("pipe failed\n");
	}
	i = -1;
	if (sh->tokens && sh->tokens[0]
		&& ft_strncmp(sh->tokens[0], "exit", 5) == 0)
	{
		write(1, "exit\n", 5);
		ft_free(sh->tokens);
		ft_free(sh->env);
		if (sh->input)
		{
			free(sh->input);
			sh->input = NULL;
		}
		clear_history();
		exit(0);
	}
	while (++i < cmd_count)
	{
		var.pids[i] = fork();
		if (var.pids[i] < 0)
			ft_exit("fork failed\n");
		else if (var.pids[i] == 0)
		{
			if (i != 0)
				dup2(var.pfd[(i - 1) * 2], 0);
			if (i != cmd_count - 1)
				dup2(var.pfd[i * 2 + 1], 1);
			j = -1;
			while (++j < 2 * sh->pipe_count)
				close(var.pfd[j]);
			var.path = find_cmd(sh->tokens[i * 2], sh->env, 0, NULL);
			if (!var.path)
				ft_exit("\033[1;31mCommand not found");
			var.cmd = ft_split(sh->tokens[i * 2], ' ');
			if (!var.cmd)
			{
				free(var.path);
				ft_exit("\033[1;31mError(malloc):\n");
			}
			if (execve(var.path, var.cmd, sh->env) == -1)
			{
				ft(var.cmd, var.path);
				ft_exit("\033[1;31mexecve cmd1");
			}
			ft_exit("execve failed\n");
		}
	}
	i = -1;
	while (++i < 2 * sh->pipe_count)
		close(var.pfd[i]);
	i = -1;
	while (++i < cmd_count)
		waitpid(var.pids[i], NULL, 0);

	free(var.pfd);
	free(var.pids);
	return (0);
}

int	minishell_start(t_shell *shell)
{
	while (1)
	{
		shell->input = readline("\033[1;34mMiniShell $ \033[0;37m");
		if (!shell->input)
			break ;
		if (shell->input && *shell->input != '\0')
		{
			if (token(shell))
				continue ;
			add_history(shell->input);
			start(shell);
			ft_free(shell->tokens);
		}
	}
	clear_history();
	return (0);
}

#include "minishell.h"

void	ft_exit(char *err) //!
{
	perror(err);
	exit(1);
}

char	*find_cmd(char *command, char **envp)
{
	char	**paths;
	int		i;
	char	*try;
	char	*full;

	if (!command || !*command)
		return (NULL);
	if (ft_strchr(command, '/'))
	{
		if (access(command, X_OK) == 0)
			return (ft_strdup(command));
		return (NULL);
	}
	paths = NULL;
	i = 0;
	while (envp && envp[i] && ft_strncmp(envp[i], "PATH=", 5))
		i++;
	if (envp && envp[i])
		paths = ft_split(envp[i] + 5, ':');
	if (!paths)
		return (NULL);
	i = 0;
	try = NULL;
	while (paths[i])
	{
		try = ft_strjoin(paths[i], "/", 0);
		full = ft_strjoin(try, command, 1);
		if (!full)
			return (ft_free(paths), NULL);
		if (access(full, X_OK) == 0)
		{
			ft_free(paths);
			return (full);
		}
		free(full);
		i++;
	}
	ft_free(paths);
	return (NULL);
}

char **split_cmds_by_pipe(char **tokens, int *cmd_count)
{
	if (!tokens) return NULL;
	int i = 0;
	int cmds = 1;
	while (tokens[i]) { if (strcmp(tokens[i], "|") == 0) cmds++; i++; }
	*cmd_count = cmds;
	char **res = malloc(sizeof(char *) * (cmds + 1));
	if (!res) return NULL;
	int cur = 0;
	size_t cap = 128;
	char *buf = malloc(cap);
	if (!buf)
	{ free(res); return NULL; }
	buf[0] = '\0';
	size_t blen = 0;
	i = 0;
	while (tokens[i])
	{
		if (strcmp(tokens[i], "|") == 0)
		{
			buf[blen] = '\0';
			res[cur++] = ft_strdup(buf);
			blen = 0;
			buf[0] = '\0';
		}
		else
		{
			size_t need = strlen(tokens[i]);
			if (blen != 0)
			{
				if (blen + 1 + need + 1 >= cap)
				{
					cap = (blen + need + 64);
					buf = realloc(buf, cap);
				}
				buf[blen++] = ' ';
			}
			if (blen + need + 1 >= cap)
			{
				cap = (blen + need + 64);
				buf = realloc(buf, cap);
			}
			memcpy(buf + blen, tokens[i], need);
			blen += need;
			buf[blen] = '\0';
		}
		i++;
	}
	buf[blen] = '\0';
	res[cur++] = ft_strdup(buf);
	res[cur] = NULL;
	free(buf);
	return res;
}

char	**split_args(const char *cmd)
{
	if (!cmd) return NULL;
	int i = 0;
	int count = 0;
	while (cmd[i])
	{
		while (cmd[i] == ' ') i++;
		if (!cmd[i]) break;
		count++;
		while (cmd[i] && cmd[i] != ' ') i++;
	}
	char **argv = malloc(sizeof(char *) * (count + 1));
	if (!argv) return NULL;
	i = 0;
	int j = 0;
	while (cmd[i])
	{
		while (cmd[i] == ' ') i++;
		if (!cmd[i]) break;
		int start = i;
		while (cmd[i] && cmd[i] != ' ') i++;
		argv[j++] = ft_substr(cmd, start, i - start);
	}
	argv[j] = NULL;
	return argv;
}

int start(t_shell *sh)
{
	int cmd_count = 0;
	char **cmds = split_cmds_by_pipe(sh->tokens, &cmd_count);
	if (!cmds) return 1;
	char **first_argv = split_args(cmds[0]);
	if (first_argv && first_argv[0] && strcmp(first_argv[0], "exit") == 0)
	{
		ft_free(first_argv);
		ft_free(cmds);
		write(1, "exit\n", 5);
		exit(0);
	}
	ft_free(first_argv);

	int (*pfd)[2] = malloc(sizeof(int[2]) * (cmd_count - 1 > 0 ? cmd_count - 1 : 1));
	pid_t *pids = malloc(sizeof(pid_t) * cmd_count);
	if ((cmd_count - 1) > 0 && !pfd) ft_exit_perror("pipe malloc");
	if (!pids) ft_exit_perror("malloc");

	for (int i = 0; i < cmd_count; ++i)
	{
		if (i < cmd_count - 1)
		{
			if (pipe(pfd[i]) == -1) ft_exit_perror("pipe");
		}
		pids[i] = fork();
		if (pids[i] < 0)
			ft_exit_perror("fork");
		if (pids[i] == 0)
		{
			if (i > 0)
			{
				dup2(pfd[i - 1][0], STDIN_FILENO);
			}
			if (i < cmd_count - 1)
			{
				dup2(pfd[i][1], STDOUT_FILENO);
			}
			for (int k = 0; k < cmd_count - 1; ++k)
			{
				close(pfd[k][0]); close(pfd[k][1]);
			}
			char **argv = split_args(cmds[i]);
			if (!argv || !argv[0])
			{
				ft_free(argv);
				ft_exit_perror("invalid cmd");
			}
			char *path = find_cmd(argv[0], sh->env);
			if (!path)
			{
				ft_free(argv);
				write(2, "Command not found\n", 18);
				exit(127);
			}
			execve(path, argv, sh->env);
			perror("execve");
			free(path);
			ft_free(argv);
			exit(126);
		}
		// parent
		if (i > 0)
		{
			close(pfd[i-1][0]); close(pfd[i-1][1]);
		}
	}
	//if (cmd_count - 1 > 0) {}
	int status = 0;
	for (int i = 0; i < cmd_count; ++i)
		waitpid(pids[i], &status, 0);
	free(pids);
	free(pfd);
	ft_free(cmds);
	return (0);
}

char	*no_spaces(char *str)
{
	int		i;
	int		start;
	char	*result;

	i = 0;
	while (str[i] && (str[i] == ' ' || str[i] == '\t'))
		i++;
	start = i;
	while (str[i] && (str[i] != ' ' || str[i] != '\t'))
		i++;
	result = ft_substr(str, start, i - start);
	return(result);
}

void	minishell_start(t_shell *shell)
{
	char	*space_free;

	while (1)
	{
		shell->input = readline("\033[1;34mMiniShell $ \033[1;36m");
		if (!shell->input)
			break ;
		if (*shell->input == '\0')
		{
			free(shell->input);
			continue ;
		}
		space_free = no_spaces(shell->input);
		if (strncmp(space_free, "$?", 3) == 0)
		{
			printf("shell: %d: command not found\n", g_exit_status);
			add_history(shell->input);
			free(shell->input);
			free(space_free);
			continue ;
		}
		if (token(shell, 0, 0))
		{
			ft_free(shell->tokens);
			free(shell->input);
			free(space_free);
			continue ;
		}
		add_history(shell->input);
		start(shell);
		free(shell->input);
		free(space_free);
		ft_free(shell->tokens);
	}
	rl_clear_history();
}

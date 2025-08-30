#include "minishell.h"

char	*ft_join(char *a, char *b, char *c)
{
	char	*res;

	if (a == NULL)
		res = ft_strjoin("shell: ", b, 0);
	else
	{
		res = ft_strjoin(a, ": ", 0);
		res = ft_strjoin(res, b, 1);
	}
	res = ft_strjoin(res, c, 1);
	return (res);
}

static int	redir_heredoc(char *delimiter, t_exec *data, t_shell *sh)
{
	int		p[2];
	char	*line;
	char	**tmp;
	int		i;
	char	*path;

	i = 1;
	if (pipe(p) == -1)
		ft_exit_perror("pipe");
	tmp = ft_split(delimiter, ' ');
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strncmp(line, tmp[0], ft_strlen(tmp[0]) + 1) == 0)
		{
			free(line);
			break ;
		}
		write(p[1], line, ft_strlen(line));
		write(p[1], "\n", 1);
		free(line);
	}
	close(p[1]);
	if (!tmp[1])
		return (p[0]);
	path = find_cmd(data->cmd, sh->env, 0, NULL);
	if (tmp[1] && data->cmd[0] == '-')
		printf("shell: %s: command not found\n", tmp[1]);
	else if (tmp[1] && data->cmd)
	{
		if (!path)
		{
			printf("shell: %s: command not found\n", data->cmd);
			exit (127);
		}
		while (tmp[i])
			printf("%s: %s: No such file or directory\n", data->cmd, tmp[i++]);
	}
	else if (!tmp[1] && !path && data->cmd && data->cmd[0] != '-')
		printf("shell: %s: command not found\n", data->cmd);
	if (path)
		free(path);
	close(p[0]);
	free(tmp);
	return (-1);
}


static char	*redir_heredoc_2(char *delimiter, t_exec *data, t_shell *sh)
{
	int		p[2];
	char	*line;
	char	**tmp;
	char	*res_tmp;
	char	*path;
	char	*res;
	int		i;

	i = 1;
	res = NULL;
	res_tmp = NULL;
	if (pipe(p) == -1)
		ft_exit_perror("pipe");
	tmp = ft_split(delimiter, ' ');
	restore_std(sh);
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strncmp(line, tmp[0], ft_strlen(tmp[0]) + 1) == 0)
		{
			free(line);
			break ;
		}
		write(p[1], line, ft_strlen(line));
		write(p[1], "\n", 1);
		free(line);
	}
	close(p[1]);
	dup2(p[0], STDIN_FILENO);
	close(p[0]);
	path = find_cmd(data->cmd, sh->env, 0, NULL);
	if (tmp[1] && data->cmd[0] == '-')
		res = ft_join(NULL, tmp[1], ": command not found\n");
	else if (tmp[1] && data->cmd)
	{
		if (!path)
		{
			res = ft_join(NULL, data->cmd, ": command not found\n");
			g_exit_status = 127;
			ft_free(tmp);
			return (res);
		}
		res = ft_strdup("");
		while (tmp[i])
		{
			res_tmp = ft_join(data->cmd, tmp[i++], ": No such file or directory\n");
			res = ft_strjoin(res, res_tmp, 1);
			free(res_tmp);
		}
	}
	else if (!tmp[1] && !path && data->cmd && data->cmd[0] != '-')
		res = ft_join(NULL, data->cmd, ": command not found\n");
	ft_free(tmp);
	if (path)
		free(path);
	return (res);
}

void	herdoc_handle(t_shell *sh, t_exec **data)
{
	t_exec	*cur;
	t_exec	*prev;
	t_exec	*next;
	int		count;
	char	*print;
	char	*tmp;

	if (sh->heredocs == 0)
		return ;
	prev = NULL;
	cur = *data;
	print = ft_strdup("");
	count = 0;
	while (cur && cur->cmd)
	{
		count++;
		if (sh->heredocs > 1 && cur->token && ft_strncmp(cur->token, "<<", 2) == 0)
		{
			tmp = redir_heredoc_2(cur->cmd2, cur, sh);
			if (tmp)
			{
				print = ft_strjoin(print, tmp, 1);
				free(tmp);
			}
			next = cur->next;
			if (prev)
				prev->next = next;
			else
				*data = next;
			free(cur);
			cur = next;
			count--;
			continue ;
		}
		else if (sh->heredocs == 1 && cur->token && ft_strncmp(cur->token, "<<", 2) == 0)
		{
			(*data)->heredoc_fd = redir_heredoc((*data)->cmd2, *data, sh);
			return ;
		}
		prev = cur;
		cur = cur->next;
	}
	sh->pipe_count = count - 1;
}

void	redirections_execve(t_exec *cmds, t_vars *vars, int i, t_shell *sh)
{
	if (i > 0)
	{
		if (dup2(vars->pfd[i - 1][0], STDIN_FILENO) < 0)
			ft_exit_perror("dup2 stdin");
	}
	if (i < sh->pipe_count)
	{
		if (dup2(vars->pfd[i][1], STDOUT_FILENO) < 0)
			ft_exit_perror("dup2 stdout");
	}
	i = -1;
	while (++i < sh->pipe_count)
	{
		close(vars->pfd[i][0]);
		close(vars->pfd[i][1]);
	}
	handle_redirection(cmds);
	if (cmds->heredoc_fd > 0)
	{
		dup2(cmds->heredoc_fd, STDIN_FILENO);
		close(cmds->heredoc_fd);
	}
	if (ft_strncmp(cmds->cmd, "-", 1) == 0)
		exit(0);
	vars->path = find_cmd(cmds->cmd, sh->env, 0, NULL);
	vars->cmd = ft_split(cmds->cmd, ' ');
	if (!vars->path)
	{
		restore_std(sh);
		ft_putstr_fd("shell: ", STDERR_FILENO);
		ft_putstr_fd(vars->cmd[0], STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		ft_free(vars->cmd);
		exit (127);
	}
	execve(vars->path, vars->cmd, sh->env);
	free(vars->path);
	ft_free(vars->cmd);
	exit (1);
}

void	restore_std(t_shell *sh)
{
	if (dup2(sh->stdin_backup, STDIN_FILENO) == -1)
		ft_exit_perror("dup2 restore stdin");
	if (dup2(sh->stdout_backup, STDOUT_FILENO) == -1)
		ft_exit_perror("dup2 restore stdout");
}


int	start(t_shell *sh)
{
	t_exec	*cmds;
	t_exec	*current;
	int		status;
	t_vars	vars;
	int		i;

	cmds = split_by_pipe(sh, 0, NULL);
	if (!cmds)
	{
		ft_free(sh->tokens);
		g_exit_status = 1;
		return (1);
	}
	herdoc_handle(sh, &cmds);
	if (!cmds)
	{
		ft_free(sh->tokens);
		ft_free_execs(cmds);
		g_exit_status = 1;
		return (1);
	}
	if (sh->pipe_count > 0)
		vars.pfd = malloc(sizeof(int [2]) * sh->pipe_count);
	else
		vars.pfd = NULL;
	vars.pids = malloc(sizeof(pid_t) * (sh->pipe_count + 1));
	if (sh->pipe_count > 0 && !vars.pfd)
		ft_exit_perror("pipe malloc");
	if (!vars.pids)
		ft_exit_perror("pids malloc");
	i = 0;
	while (i < sh->pipe_count)
	{
		if (pipe(vars.pfd[i]) == -1)
			ft_exit_perror("pipe failed");
		i++;
	}
	i = 0;
	current = cmds;
	while (current->cmd && i < sh->pipe_count + 1)
	{
		vars.pids[i] = fork();
		if (vars.pids[i] < 0)
			ft_exit_perror("fork failed");
		if (vars.pids[i] == 0)
			redirections_execve(current, &vars, i, sh);
		if (i > 0)
		{
			close(vars.pfd[i - 1][0]);
			close(vars.pfd[i - 1][1]);
		}
		if (current->next)
			current = current->next;
		i++;
	}
	i = -1;
	while (++i < sh->pipe_count)
	{
		close(vars.pfd[i][0]);
		close(vars.pfd[i][1]);
	}
	i = -1;
	while (++i < sh->pipe_count + 1)
		waitpid(vars.pids[i], &status, 0);
	ft_free_execs(cmds);
	if (vars.pfd)
		free(vars.pfd);
	if (vars.pids)
		free(vars.pids);
	g_exit_status = WEXITSTATUS(status);
	return (0);
}

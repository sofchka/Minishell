#include "minishell.h"

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
	if (handle_redirection(cmds))
		ft_exit_perror("redirection failed");
	if (is_builtin(cmds->cmd))
	{
		if (execute_builtin(cmds->cmd, sh) == -1)
			ft_exit_perror("builtin execution failed");
	}
	vars->path = find_cmd(cmds->cmd, sh->env, 0, NULL);
	if (!vars->path)
		ft_exit_perror("command not found");
	vars->cmd = ft_split(cmds->cmd, ' ');
	if (!vars->cmd)
	{
		free(vars->path);
		ft_exit_perror("split args failed");
	}
	execve(vars->path, vars->cmd, sh->env);
	free(vars->path);
	ft_free(vars->cmd);
	ft_exit_perror("execve failed");
}

int	start(t_shell *sh)
{
	t_exec	*cmds;
	t_exec	*current;
	int		status;
	t_vars	vars;
	int		i;

	cmds = split_by_pipe(sh);
	if (sh->pipe_count == 0 && cmds && cmds->cmd && cmds->next == NULL)
	{
		if (ft_strncmp(cmds->cmd, "exit", ft_strlen(cmds->cmd) + 1) == 0)
			return (ft_exit(ft_split(cmds->cmd, ' '), sh));
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
		printf("Creating child process %d for command: %s %s %s\n", i, current->cmd, current->token ? current->token : "X", current->cmd2 ? current->cmd2 : "X");
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

	write(1, "WOW finished worked pipe done\n", 30);
	return (0);
}

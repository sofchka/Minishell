#include "minishell.h"

static void	init_vars(t_shell *sh, t_vars *vars)
{
	int	i;

	if (sh->pipe_count > 0)
		vars->pfd = malloc(sizeof(int [2]) * sh->pipe_count);
	else
		vars->pfd = NULL;
	vars->pids = malloc(sizeof(pid_t) * (sh->pipe_count + 1));
	if (sh->pipe_count > 0 && !vars->pfd)
		ft_exit_perror("pipe malloc");
	if (!vars->pids)
		ft_exit_perror("pids malloc");
	i = 0;
	while (i < sh->pipe_count)
	{
		if (pipe(vars->pfd[i]) == -1)
			ft_exit_perror("pipe failed");
		i++;
	}
}

static void	exec_child(t_shell *sh, t_vars *vars, t_exec *current, int i)
{
	if (current && current->cmd)
	{
		vars->pids[i] = fork();
		if (vars->pids[i] < 0)
			ft_exit_perror("fork failed");
		if (vars->pids[i] == 0)
			redirections_execve(current, vars, i, sh);
	}
}

static void	exec_all(t_shell *sh, t_vars *vars, t_exec *cmds)
{
	t_exec	*current;
	int		i;

	current = cmds;
	i = 0;
	while (i < sh->pipe_count + 1 && current)
	{
		if (current->cmd)
			exec_child(sh, vars, current, i);
		else if (!current->cmd && current->token
			&& !ft_strncmp(current->token, "<<", 3))
			handle_redirection(current);
		if (i > 0)
		{
			close(vars->pfd[i - 1][0]);
			close(vars->pfd[i - 1][1]);
		}
		if (current->next)
			current = current->next;
		i++;
	}
}

static void	wait_and_clean(t_shell *sh, t_vars *vars,
			t_exec *cmds, int status)
{
	int	i;

	i = -1;
	while (++i < sh->pipe_count)
	{
		close(vars->pfd[i][0]);
		close(vars->pfd[i][1]);
	}
	i = -1;
	while (++i < sh->pipe_count + 1)
		waitpid(vars->pids[i], &status, 0);
	ft_free_execs(cmds);
	if (vars->pfd)
		free(vars->pfd);
	if (vars->pids)
		free(vars->pids);
	g_exit_status = WEXITSTATUS(status);
}

int	start(t_shell *sh)
{
	t_exec	*cmds;
	t_vars	vars;
	char	**split_arg;
	int		status;

	status = 0;
	cmds = split_by_pipe(sh, 0, new_exec_node(), NULL);
	if (sh->heredocs > 0)
		herdoc_handle(sh, &cmds, 0);
	if (!cmds)
		return (0);
	if (cmds->cmd)
	{
		split_arg = ft_split(cmds->cmd, ' ');
		if (sh->pipe_count == 0 && cmds && is_builtin(split_arg[0]))
		{
			g_exit_status = exec_builtin(sh, split_arg, cmds);
			ft_free_execs(cmds);
			return (0);
		}
	}
	init_vars(sh, &vars);
	exec_all(sh, &vars, cmds);
	wait_and_clean(sh, &vars, cmds, status);
	return (0);
}

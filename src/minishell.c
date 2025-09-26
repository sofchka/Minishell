#include "minishell.h"

int	start(t_shell *sh)
{
	t_exec	*cmds;
	t_exec	*current;
	int		status;
	t_vars	vars;
	int		i;

	cmds = split_by_pipe(sh, 0, NULL);
	if (sh->heredocs > 0)
		herdoc_handle(sh, &cmds, 0);
	char **split_arg = ft_split(cmds->cmd, ' ');
	if (sh->pipe_count == 0 && cmds && is_builtin(split_arg[0]))
	{
		g_exit_status = exec_builtin(sh, split_arg, cmds);
		ft_free_execs(cmds);
		return (0);
	}
	if (!cmds)
		return (0);
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

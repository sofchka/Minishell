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
	handle_redirection(cmds);
	if (cmds->heredoc_fd > 0)
	{
		dup2(cmds->heredoc_fd, STDIN_FILENO);
		close(cmds->heredoc_fd);
	}
	if (ft_strncmp(cmds->cmd, "-", 2) == 0
		&& (cmds->token && ft_strncmp(cmds->token, "<<", 3) == 0))
		exit(0);
	if (ft_strncmp(cmds->cmd, ".", 2) == 0)
	{
		restore_std(sh);
		ft_putstr_fd("shell: .: filename argument required\n", STDERR_FILENO);
		ft_putstr_fd(".: usage: . filename [arguments]\n", STDERR_FILENO);
		exit(2);
	}
	if (ft_strncmp(cmds->cmd, "..", 3) == 0)
	{
		restore_std(sh);
		print_error(cmds->cmd, "command not found");
		exit(127);
	}
	vars->path = find_cmd(cmds->cmd, sh->env, 0, NULL);
	vars->cmd = ft_split(cmds->cmd, ' ');
	if (!vars->path)
	{
		restore_std(sh);
		if (ft_strchr(vars->cmd[0], '/'))
			print_error(vars->cmd[0], "No such file or directory");
		else
			print_error(vars->cmd[0], "command not found");
		ft_free(vars->cmd);
		exit (127);
	}
	struct stat st;
	if (stat(vars->path, &st) == 0 && S_ISDIR(st.st_mode))
	{
		restore_std(sh);
		print_error(cmds->cmd, "is a directory");
		free(vars->path);
		exit(126);
	}
	errno = 0;
	execve(vars->path, vars->cmd, sh->env);
	restore_std(sh);
	if (errno == ENOENT)
		print_error(cmds->cmd, "No such file or directory");
	else if (errno == EACCES)
		print_error(cmds->cmd, "Permission denied");
	else if (errno == ENOEXEC)
		print_error(cmds->cmd, "Exec format error");
	else
		perror("shell");

	free(vars->path);
	ft_free(vars->cmd);
	if (errno == ENOENT)
		exit (127);
	exit(126);
}

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

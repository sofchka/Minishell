#include "minishell.h"

static int	redir_input(char *file, t_exec *cmds)
{
	int		fd;
	char	*name;

	name = strip_quotes(file);
	fd = open(name, O_RDONLY);
	free(name);
	if (fd < 0)
	{
		ft_exit_perror(file);
		g_exit_status = 1;
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) < 0)
		return (close(fd), ft_exit_perror("dup2"), g_exit_status = 1, -1);
	close(fd);
	cmds->has_infile = 1;
	return (0);
}

int	redir_output(char *file, char *type, t_exec *cmds)
{
	int		fd;
	char	*name;

	name = strip_quotes(file);
	if (!ft_strncmp(type, ">>", 3))
		fd = open(name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	free(name);
	if (fd < 0)
		return (ft_exit_perror(file), g_exit_status = 1, -1);
	if (dup2(fd, STDOUT_FILENO) < 0)
		return (close(fd), ft_exit_perror("dup2"), g_exit_status = 1, -1);
	close(fd);
	cmds->has_outfile = 1;
	return (0);
}

int	handle_redirection(t_exec *cmds)
{
	if (!cmds->token || !cmds->cmd2)
		return (0);
	if (!ft_strncmp(cmds->token, "<", 2))
		return (redir_input(cmds->cmd2, cmds));
	if (!ft_strncmp(cmds->token, ">", 2)
		|| !ft_strncmp(cmds->token, ">>", 3))
		return (redir_output(cmds->cmd2, cmds->token, cmds));
	return (0);
}

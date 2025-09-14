#include "minishell.h"

void	ft_free(char **arg)
{
	int	i;

	if (!arg)
		return ;
	i = 0;
	while (arg[i])
	{
		free(arg[i]);
		i++;
	}
	free(arg);
}

void	ft_exit_perror(const char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

void	ft_free_execs(t_exec *cmds)
{
	t_exec	*tmp;

	while (cmds)
	{
		tmp = cmds->next;
		if (cmds->heredoc_fd > 0)
			close(cmds->heredoc_fd);
		// if (cmds->cmd[0] != '-')
		// 	ft_free(&(cmds->cmd));
		free(cmds);
		cmds = tmp;
	}
}


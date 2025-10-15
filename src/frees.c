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
	if (errno == EPIPE)
		exit(1);
	perror(msg);
	exit(EXIT_FAILURE);
}

void	ft_free_execs(t_exec *cmds)
{
	t_exec	*tmp;
	t_rsub	*n;

	while (cmds)
	{
		tmp = cmds->next;
		if (cmds->heredoc_fd > 0)
			close(cmds->heredoc_fd);
		free(cmds->cmd);
		free(cmds->cmd2);
		while (cmds->subs)
		{
			n = cmds->subs->next;
			free(cmds->subs->op);
			free(cmds->subs->arg);
			free(cmds->subs);
			cmds->subs = n;
		}
		free(cmds);
		cmds = tmp;
	}
}


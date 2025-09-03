#include "minishell.h"

static void	write_heredoc_input(int fd, char *delimiter)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line || ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) == 0)
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
}

static char	*heredoc_check_cmd(t_exec *data, char **tmp, t_shell *sh)
{
	char	*res;
	char	*path;
	int		i;

	i = 1;
	res = NULL;
	path = find_cmd(data->cmd, sh->env, 0, NULL);
	if (tmp[1] && data->cmd[0] == '-')
		res = ft_join(NULL, tmp[1], ": command not found\n");
	else if (tmp[1] && data->cmd)
	{
		if (!path)
			return (ft_join(NULL, data->cmd, ": command not found\n"));
		res = ft_strdup("");
		while (tmp[i])
			res = ft_strjoin(res, ft_join(data->cmd, tmp[i++],
						": No such file or directory\n"), 3);
	}
	else if (!tmp[1] && !path && data->cmd && data->cmd[0] != '-')
		res = ft_join(NULL, data->cmd, ": command not found\n");
	free(path);
	return (res);
}

static char	*redir_heredoc(char *delimiter, t_exec *data,
							t_shell *sh, int attach_stdin)
{
	int		p[2];
	char	**tmp;
	char	*res;

	if (pipe(p) == -1)
		ft_exit_perror("pipe");
	tmp = ft_split(delimiter, ' ');
	write_heredoc_input(p[1], tmp[0]);
	close(p[1]);
	if (attach_stdin)
	{
		dup2(p[0], STDIN_FILENO);
		close(p[0]);
		res = heredoc_check_cmd(data, tmp, sh);
	}
	else
	{
		res = NULL;
		data->heredoc_fd = p[0];
	}
	ft_free(tmp);
	return (res);
}

static int	process_heredoc_node(t_shell *sh, t_exec **data,
	t_exec *cur, t_exec *prev)
{
	char	*msg;

	if (sh->heredocs > 1)
	{
		restore_std(sh);
		msg = redir_heredoc(cur->cmd2, cur, sh, 1);
		if (msg)
		{
			ft_putstr_fd(msg, 2);
			free(msg);
		}
		if (prev)
			prev->next = cur->next;
		else
			*data = cur->next;
		free(cur);
		return (1);
	}
	else
		redir_heredoc(cur->cmd2, cur, sh, 0);
	return (0);
}

void	herdoc_handle(t_shell *sh, t_exec **data, int count)
{
	t_exec	*cur;
	t_exec	*prev;

	prev = NULL;
	cur = *data;
	while (cur && cur->cmd)
	{
		count++;
		if (cur->token && !ft_strncmp(cur->token, "<<", 2))
		{
			if (process_heredoc_node(sh, data, cur, prev))
			{
				if (sh->heredocs > 1)
					count--;
				if (prev)
					cur = prev->next;
				else
					cur = *data;
				continue ;
			}
		}
		prev = cur;
		cur = cur->next;
	}
	sh->pipe_count = count - 1;
}

#include "minishell.h"

static int	redir_input(char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		perror(file);
		return (1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

static int	redir_output(char *file, char *type)
{
	int	fd;

	if (type[1] == '\0')
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		perror(file);
		return (1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

static int	redir_heredoc(char *delimiter)
{
	int		p[2];
	char	*line;

	if (pipe(p) == -1)
		ft_exit_perror("pipe");
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) == 0)
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
	return (0);
}

int	handle_redirection(t_exec *data)
{
	if (data->token)
	{
		if (!data->cmd2)
		{
			write(2, "\033[1;31mSyntax error: missing file after redirection\n", 50);
			return (1);
		}
		if (data->token[0] == '<' && data->token[1] == '\0')
			return (redir_input(data->cmd2));
		if ((data->token[0] == '>' && data->token[1] == '\0')
			|| (data->token[0] == '>' && data->token[1] == '>'))
			return (redir_output(data->cmd2, data->token));
		if (data->token[0] == '<' && data->token[1] == '<')
			return (redir_heredoc(data->cmd2));
	}
	return (0);
}

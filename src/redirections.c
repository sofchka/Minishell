#include "minishell.h"

static int	redir_input(char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		perror(file);
		exit (1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

int	redir_output(char *file, char *type, int dup)
{
	int	fd;

	if (type[1] == '\0')
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		perror(file);
		exit (1);
	}
	if (dup)
		dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

int	handle_redirection(t_exec *data)
{
	if (data->token)
	{
		if (!data->cmd2)
		{
			write(2, "Syntax error: missing file after redirection\n", 50);
			return (1);
		}
		if (data->token[0] == '<' && data->token[1] == '\0')
			return (redir_input(data->cmd2));
		if ((data->token[0] == '>' && data->token[1] == '\0')
			|| (data->token[0] == '>' && data->token[1] == '>'))
			return (redir_output(data->cmd2, data->token, 1));
	}
	return (0);
}

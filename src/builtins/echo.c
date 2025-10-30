#include "../minishell.h"

int	is_n_e(char *str)
{
	while (*str && ((*str >= '\t' && *str <= '\r') || *str == ' '))
		str++;
	if (*str != '-')
		return (0);
	str++;
	if (*str == '\0')
		return (0);
	while (*str)
	{
		if (*str != 'n' && *str != 'e')
			return (0);
		str++;
	}
	return (1);
}

int	ft_echo(t_shell *sh, char **cmd)
{
	int	flag;
	int	arg;
	int	i;

	flag = 0;
	arg = 1;
	(void)sh;
	while (cmd[arg] && is_n_e(cmd[arg]))
	{
		flag = 1;
		arg++;
	}
	while (cmd[arg])
	{
		i = 0;
		while (cmd[arg][i])
			write(1, &cmd[arg][i++], 1);
		arg++;
		if (cmd[arg])
			write(1, " ", 1);
	}
	if (!flag)
		write(1, "\n", 1);
	return (0);
}

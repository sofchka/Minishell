#include "minishell.h"

long	ft_atol(const char *str)
{
	long	result;
	int		sign;
	int		i;

	i = 0;
	sign = 1;
	result = 0;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n')
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (sign * result);
}

void	ft_exit_loop(char **argv)
{
	int		i;

	i = 0;
	while (argv[1][i])
	{
		if (i == 0 && (argv[1][i] == '-' || argv[1][i] == '+'))
			i++;
		if (!ft_isdigit(argv[1][i]))
		{
			ft_putstr_fd("exit: ", STDERR_FILENO);
			ft_putstr_fd(argv[1], STDERR_FILENO);
			ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
			exit(255);
		}
		i++;
	}
}


int	ft_exit(char **argv, t_shell *shell)
{
	int		status;
	long	num;

	status = g_exit_status;
	ft_putstr_fd("exit\n", STDOUT_FILENO);
	if (argv[0] && argv[1])
	{
		if (argv[2])//when exit NUM strings -> too many args
		{//but if exit strings -> exit with print numeric needed
			ft_putstr_fd("exit: too many arguments\n", STDERR_FILENO);
			exit(1); //or return(1); (for my mac its return)
		}
		num = ft_atol(argv[1]);
		ft_exit_loop(argv);
		status = (int)(num % 256);
		if (status < 0)
			status += 256;
	}
	ft_free(shell->env);
	free(shell->input);
	ft_free(shell->tokens);
	close(shell->stdin_backup);
	close(shell->stdout_backup);
	exit(status);
}

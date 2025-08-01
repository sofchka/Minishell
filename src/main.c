#include "minishell.h"

int	g_exit_status = 0;

void	ft_free(char **arg)
{
	int	i;

	if (!arg || !*arg)
		return ;
	i = 0;
	while (arg[i])
	{
		free(arg[i]);
		i++;
	}
	free(arg);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	if (argc != 1)
	{
		write(2, "No arguments needed\n", 21);
		return (1);
	}
	(void)argv;
	init_shell(&shell);
	shell.env = dup_env(envp);
	if (!shell.env)
	{
		write(2, "Env init Error\n", 15);
		return (1);
	}
	//set_signals();
	if (minishell_start(&shell))
	{
		ft_free(shell.env);
		return (1);
	}
	ft_free(shell.env);
	return (0);
}

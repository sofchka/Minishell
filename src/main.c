#include "minishell.h"

int	g_exit_status = 0;

void	ft_exit_perror(const char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

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

static void	handling_signal(int signal)//?
{
	if (signal == SIGINT)
	{
		g_exit_status = 130;
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	set_signals(void)
{
	signal(SIGINT, &handling_signal);
	signal(SIGQUIT, SIG_IGN);
}


int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argv;
	if (argc != 1)
	{
		write(2, "\033[1;31mNo arguments needed\n", 28);
		return (1);
	}
	init_shell(&shell);
	shell.env = dup_env(envp);
	if (!shell.env)
	{
		write(2, "\033[1;31mEnv init Error\n", 23);
		return (1);
	}
	set_signals();
	minishell_start(&shell);
	ft_free(shell.env);
	return (0);
}

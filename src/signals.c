#include "minishell.h"

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

void	sigint_heredoc(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	close(STDIN_FILENO);
	g_exit_status = 130;
}

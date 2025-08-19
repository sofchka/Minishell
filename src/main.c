#include "minishell.h"

int g_exit_status;

void	main_loop(t_shell *shell)
{
	char	*expanded;

	while (1)
	{
		shell->input = readline("\033[1;34mMiniShell $ \033[1;36m");
		if (!shell->input)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			break ;
		}
		if (*shell->input == '\0')
		{
			free(shell->input);
			continue ;
		}
		add_history(shell->input);
		expanded = expand_vars(shell->input, shell);
		free(shell->input);
		shell->input = expanded;
		if (token(shell, 0, 0))
		{
			ft_free(shell->tokens);
			free(shell->input);
			continue ;
		}
		if (start(shell) == -1)
			break ;
		shell->tok_count = 0;
		shell->pipe_count = 0;
		free(shell->input);
		ft_free(shell->tokens);
	}
	rl_clear_history();
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argv;
	g_exit_status = 0;
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
	main_loop(&shell);
	ft_free(shell.env);
	return (0);
}

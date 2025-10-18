#include "minishell.h"

int g_exit_status;

void	main_loop(t_shell *shell, char *expanded)
{
	while (1)
	{
		shell->input = readline("\033[1;34mMiniShell $ \033[1;36m");
		if (!shell->input)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			free(shell->input);
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
			free(shell->input);
			continue ;
		}
		if (!shell->tokens || !shell->tokens[0])
		{
			free(shell->input);
			continue ;
		}
		// int i = 0;
		// while (shell->tokens[i])
		// {
		// 	printf(" [%s]\n", shell->tokens[i]);
		// 	i++;
		// }
		if (start(shell, 0) == -1)
			break ;
		reinit(&shell);
	}
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
	init_env(&shell, envp);
	shell.home = get_env_value("HOME", &shell);
	if (!shell.t_env)
	{
		write(2, "\033[1;31mEnv init Error\n", 23);
		return (1);
	}
	set_signals();
	main_loop(&shell, NULL);
	rl_clear_history();
	free_env(shell.t_env);
	free(shell.home);
	return (g_exit_status);
}

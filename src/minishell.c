#include "minishell.h"

int	minishell_start(t_shell *shell)
{
	while (1)
	{
		shell->input = readline("\033[1;34mMiniShell $ \033[1;36m");
		if (!shell->input)
			break ;
		if (shell->input && *shell->input != '\0')
		{
			if (token(shell))
				continue;
			
			// int i = 0;
			// int j;
			// while (shell->tokens[i])
			// {
			// 	j = 0;
			// 	while (shell->tokens[i][j])
			// 	{
			// 		write(1, &shell->tokens[i][j], 1);
			// 		j++;
			// 	}
			// 	i++;
			// 	write(1, "\n", 1);
			// }
			add_history(shell->input);
		}
	}
	return (0);
}

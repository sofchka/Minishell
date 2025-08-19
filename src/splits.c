#include "minishell.h"


t_exec	*split_by_pipe(t_shell *sh)
{
	int		i;
	char	*token;
	t_exec	*head;
	t_exec	*cur;

	i = 0;
	token = NULL;
	head = malloc(sizeof(t_exec));
	if (!head)
		return (NULL);
	cur = head;
	cur->next = NULL;
	cur->cmd = NULL;
	cur->cmd2 = NULL;
	cur->token = NULL;
	while (sh->tokens[i])
	{
		if (ft_strncmp(sh->tokens[i], "<<", 2) == 0
			|| ft_strncmp(sh->tokens[i], ">>", 2) == 0
			|| ft_strncmp(sh->tokens[i], "<", 1) == 0
			|| ft_strncmp(sh->tokens[i], ">", 1) == 0)
		{
			token = sh->tokens[i];
			i++;
		}
		if (ft_strncmp(sh->tokens[i], "|", 1) == 0)
		{
			if (token)
			{
				cur->cmd = sh->tokens[i - 3];
				cur->cmd2 = sh->tokens[i - 1];
				cur->token = token;
				token = NULL;
			}
			else
			{
				cur->cmd = sh->tokens[i - 1];
				cur->cmd2 = NULL;
				cur->token = NULL;
			}
			cur->next = malloc(sizeof(t_exec));
			cur = cur->next;
			cur->cmd = NULL;
			cur->cmd2 = NULL;
			cur->token = NULL;
			cur->next = NULL;
		}
		i++;
	}
	if (token)
	{
		cur->cmd = sh->tokens[i - 3];
		cur->cmd2 = sh->tokens[i - 1];
		cur->token = token;
	}
	else
	{
		cur->cmd = sh->tokens[i - 1];
		cur->cmd2 = NULL;
		cur->token = NULL;
	}
	return (head);
}

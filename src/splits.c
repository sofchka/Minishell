#include "minishell.h"

static t_exec	*new_exec_node(void)
{
	t_exec	*node;

	node = malloc(sizeof(t_exec));
	if (!node)
		return (NULL);
	node->cmd = NULL;
	node->cmd2 = NULL;
	node->token = NULL;
	node->heredoc_fd = -1;
	node->next = NULL;
	return (node);
}

static void	fill_exec_node(t_exec *cur, char *token, char **tokens, int i)
{
	if (token)
	{
		if (i > 2 && ft_strncmp(tokens[i - 2], "|", 1) != 0)
			cur->cmd = tokens[i - 3];
		else
			cur->cmd = tokens[i - 2];
		cur->cmd2 = tokens[i - 1];
		cur->token = token;
	}
	else
	{
		cur->cmd = tokens[i - 1];
		cur->cmd2 = NULL;
		cur->token = NULL;
	}
}

t_exec	*split_by_pipe(t_shell *sh, int i, char *token)
{
	t_exec	*head;
	t_exec	*cur;

	head = new_exec_node();
	if (!head)
		return (NULL);
	cur = head;
	while (sh->tokens[i])
	{
		if (ft_strncmp(sh->tokens[i], "<<", 2) == 0
			|| ft_strncmp(sh->tokens[i], ">>", 2) == 0
			|| ft_strncmp(sh->tokens[i], "<", 1) == 0
			|| ft_strncmp(sh->tokens[i], ">", 1) == 0)
			token = sh->tokens[i++];
		if (sh->tokens[i] && ft_strncmp(sh->tokens[i], "|", 1) == 0)
		{
			fill_exec_node(cur, token, sh->tokens, i);
			token = NULL;
			cur->next = new_exec_node();
			cur = cur->next;
		}
		i++;
	}
	fill_exec_node(cur, token, sh->tokens, i);
	return (head);
}

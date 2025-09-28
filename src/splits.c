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
	node->has_infile = 0;
	node->has_outfile = 0;
	node->next = NULL;
	node->subs = NULL;
	return (node);
}

static int	is_pipe_tok(char *s)
{
	if (!s)
		return (0);
	return (!ft_strncmp(s, "|", 1));
}

static int	is_redir_tok(char *s)
{
	if (!s)
		return (0);
	if (!ft_strncmp(s, "<<", 2) || !ft_strncmp(s, ">>", 2))
		return (1);
	if (!ft_strncmp(s, "<", 1) || !ft_strncmp(s, ">", 1))
		return (1);
	return (0);
}

static t_rsub	*new_sub(char *op, char *arg)
{
	t_rsub	*res;

	res = malloc(sizeof(t_rsub));
	if (!res)
		return (NULL);
	res->op = ft_strdup(op);
	if (!ft_strncmp(op, "<<", 2))
		res->arg = ft_strdup(arg);
	else
		res->arg = strip_quotes(arg);
	res->next = NULL;
	return (res);
}

static int	append_sub(t_exec *cur, char *op, char *arg)
{
	t_rsub	*it;

	if (!cur || !op || !arg)
		return (0);
	if (!cur->subs)
	{
		cur->subs = new_sub(op, arg);
		return (!!cur->subs);
	}
	it = cur->subs;
	while (it->next)
		it = it->next;
	it->next = new_sub(op, arg);
	return (!!it->next);
}

static void	append_word(t_exec *cur, char *word)
{
	char	*tmp;

	if (!cur->cmd)
	{
		cur->cmd = ft_strdup(word);
		return ;
	}
	tmp = ft_strjoin(cur->cmd, " ", 1);
	cur->cmd = ft_strjoin(tmp, word, 1);
}

static int	handle_redir_token(t_exec *cur, char **tokens, int i)
{
	if (!tokens[i + 1])
		return (i);
	append_sub(cur, tokens[i], tokens[i + 1]);
	cur->token = tokens[i];
	if (!ft_strncmp(cur->token, "<<", 2))
		cur->cmd2 = ft_strdup(tokens[i + 1]);
	else
		cur->cmd2 = strip_quotes(tokens[i + 1]);
	return (i + 2);
}

t_exec	*split_by_pipe(t_shell *sh, int i, char *tok)
{
	t_exec	*head;
	t_exec	*cur;

	(void)tok;
	head = new_exec_node();
	if (!head)
		return (NULL);
	cur = head;
	while (sh->tokens[i])
	{
		if (is_pipe_tok(sh->tokens[i]))
		{
			cur->next = new_exec_node();
			cur = cur->next;
		}
		else if (is_redir_tok(sh->tokens[i]))
			i = handle_redir_token(cur, sh->tokens, i);
		else if (ft_strncmp(sh->tokens[i], " ", 2) != 0)
			append_word(cur, sh->tokens[i]);
		i++;
	}

	// t_exec	*tmp;

	// tmp = head;
	// while (tmp)
	// {
	// 	printf("NODE: cmd=[%s] cmd2=[%s] token=[%s]\n",
	// 		tmp->cmd ? tmp->cmd : "NULL",
	// 		tmp->cmd2 ? tmp->cmd2 : "NULL",
	// 		tmp->token ? tmp->token : "NULL");
	// 	tmp = tmp->next;
	// }
	return (head);
}

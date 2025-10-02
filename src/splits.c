#include "minishell.h"

static t_exec	*new_exec_node(void)
{
	t_exec	*n;

	n = malloc(sizeof(t_exec));
	if (!n)
		return (NULL);
	n->cmd = NULL;
	n->cmd2 = NULL;
	n->token = NULL;
	n->heredoc_fd = -1;
	n->has_infile = 0;
	n->has_outfile = 0;
	n->next = NULL;
	n->subs = NULL;
	return (n);
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
	t_rsub	*r;

	r = malloc(sizeof(t_rsub));
	if (!r)
		return (NULL);
	r->op = ft_strdup(op);
	if (!ft_strncmp(op, "<<", 2))
		r->arg = ft_strdup(arg);
	else
		r->arg = strip_quotes(arg);
	r->next = NULL;
	return (r);
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

static void	append_word(t_exec *cur, char *w)
{
	char	*tmp;

	if (!w || !*w)
		return ;
	if (!cur->cmd)
	{
		cur->cmd = ft_strdup(w);
		return ;
	}
	tmp = ft_strjoin(cur->cmd, " ", 1);
	cur->cmd = ft_strjoin(tmp, w, 1);
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
			i++;
			continue ;
		}
		if (is_redir_tok(sh->tokens[i]))
		{
			if (sh->tokens[i + 1])
			{
				append_sub(cur, sh->tokens[i], sh->tokens[i + 1]);
				cur->token = sh->tokens[i];
				if (!ft_strncmp(cur->token, "<<", 2))
					cur->cmd2 = ft_strdup(sh->tokens[i + 1]);
				else
					cur->cmd2 = strip_quotes(sh->tokens[i + 1]);
				i += 2;
				continue ;
			}
		}
		if (ft_strncmp(sh->tokens[i], " ", 2) != 0)
			append_word(cur, sh->tokens[i]);
		i++;
	}
	t_exec	*tmp;

	tmp = head;
	while (tmp)
	{
		printf("NODE: cmd=[%s] cmd2=[%s] token=[%s]\n",
			tmp->cmd ? tmp->cmd : "NULL",
			tmp->cmd2 ? tmp->cmd2 : "NULL",
			tmp->token ? tmp->token : "NULL");
		tmp = tmp->next;
	}
	return (head);
}

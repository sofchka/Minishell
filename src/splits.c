#include "minishell.h"

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

void	split_by_pipe_2(t_shell *sh, int *i, t_exec *cur)
{
	append_sub(cur, sh->tokens[*i], sh->tokens[*i + 1]);
	cur->token = sh->tokens[*i];
	if (!ft_strncmp(cur->token, "<<", 2))
		cur->cmd2 = ft_strdup(sh->tokens[*i + 1]);
	else
		cur->cmd2 = strip_quotes(sh->tokens[*i + 1], ft_strdup(""));
	*i += 2;
}

t_exec	*split_by_pipe(t_shell *sh, int i, t_exec *head, t_exec *cur)
{
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
				split_by_pipe_2(sh, &i, cur);
				continue ;
			}
		}
		if (ft_strncmp(sh->tokens[i], " ", 2) != 0)
			append_word(cur, sh->tokens[i]);
		i++;
	}
	return (head);
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

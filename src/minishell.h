#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <stdio.h>      // printf, perror
# include <stdlib.h>     // malloc, free, getenv, exit
# include <unistd.h>     // write, access, read, close, 
//fork, dup, dup2, isatty, ttyname, ttyslot, getcwd, chdir
# include <sys/types.h>  // opendir, readdir, wait, waitpid, 
//wait3, wait4, stat, lstat, fstat, kill, signal related
# include <sys/wait.h>   // wait, waitpid, wait3, wait4
# include <sys/stat.h>   // stat, lstat, fstat
# include <fcntl.h>      // open, unlink
# include <dirent.h>     // opendir, readdir, closedir
# include <string.h>     // strerror
# include <signal.h>     // signal, sigaction, sigemptyset, sigaddset, kill
# include <termios.h>    // tcsetattr, tcgetattr
# include <termcap.h>    // tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs
# include <sys/ioctl.h>  // ioctl
# include <readline/readline.h>   // readline, rl_clear_history, 
//rl_on_new_line, rl_replace_line, rl_redisplay
# include <readline/history.h>    // add_history
# include <errno.h>

extern int	g_exit_status;

typedef enum e_data
{
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	APPEND,
	HEREDOC,
}			t_data;

typedef struct s_exec
{
	char			*cmd;
	char			*cmd2;
	char			*token;
	int				heredoc_fd;
	struct s_exec	*next;
}			t_exec;

typedef struct s_shell
{
	char			**env;
	char			*input;
	char			**tokens;
	int				tok_count;
	int				pipe_count;
	int				heredocs;
	int				redirs;
	int				stdin_backup;
	int				stdout_backup;
	t_exec			*cmds;
}			t_shell;

typedef struct s_vars
{
	int		(*pfd)[2];
	pid_t	*pids;
	int		infile;
	int		outfile;
	char	*path;
	char	**cmd;
}	t_vars;

// frees.c
void	ft_free(char **arg);
void	ft_exit_perror(const char *msg);
void	ft_free_execs(t_exec *cmds);

// init.c
char	**dup_env(char **envp);
void	init_shell(t_shell *shell);
void	reinit(t_shell **shell);

// signals.c
void	set_signals(void);

// check.c
int		has_open_quote(const char *s);
int		syntax_error(t_shell *sh);

// token.c
int		token(t_shell *sh, int i, int j);

// redirections.c
int		handle_redirection(t_exec *data);
int		redir_output(char *file, char *type, int dup);

// splits.c
t_exec	*split_by_pipe(t_shell *sh, int i, char *token);

// find_path.c
void	ft(char **str);
int		find_path(char **envp, char ***path);
char	*find_cmd(char *command, char **envp, int i, char *tmp);

// minishell.c
void	redirections_execve(t_exec *cmds, t_vars *vars, int i, t_shell *sh);
int		start(t_shell *sh);

// expend.c
char	*expand_vars(char *input, t_shell *shell);

// utils.c
int		is_operator(const char *s, int *len, t_data *type);
void	print_error(const char *arg, const char *msg);
void	restore_std(t_shell *sh);
char	*ft_join(char *a, char *b, char *c);

// exit.c
int		ft_exit(char **argv, t_shell *shell);

// heredoc.c
void	herdoc_handle(t_shell *sh, t_exec **data, int count);

#endif

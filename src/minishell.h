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

typedef struct s_shell
{
	char			**env;
	char			*input;
	char			**tokens;
	int				tok_count;
	int				pipe_count;
	int				stdin_backup;
	int				stdout_backup;
}			t_shell;

// typedef struct s_vars
// {
// 	int		*pfd;
// 	pid_t	*pids;
// 	int		infile;
// 	int		outfile;
// 	char	*path;
// 	char	**cmd;
// }	t_vars;

void		ft_exit_perror(const char *msg);
char		**dup_env(char **envp);
void		ft_free(char **arg);
void		init_shell(t_shell *shell);
void		minishell_start(t_shell *shell);
int			token(t_shell *sh, int i, int j);
int			start(t_shell *sh);
int			has_open_quote(const char *s);
int			syntax_error(const char *input);
int			is_operator(const char *s, int *len, t_data *type);

#endif

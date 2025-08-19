#include "minishell.h"

static char	*simplify(char *str)
{
	char	*last;

	if (!str)
		return (NULL);
	last = ft_strrchr(str, '/');
	if (last && *(last + 1) != '\0')
		return (last + 1);
	return (str);
}

int	execute_builtin(char *str, t_shell *shell)
{
	char	*arg;

	arg = simplify(str);
	if (!arg || !arg[0])
		return (-1);
	if (ft_strncmp(arg, "echo", ft_strlen(arg) + 1) == 0)
		return (ft_echo(str));
	if (ft_strncmp(arg, "cd", ft_strlen(arg) + 1) == 0)
		return (ft_cd(ft_split(str, ' '), shell));
	if (ft_strncmp(arg, "pwd", ft_strlen(arg) + 1) == 0)
		return (ft_pwd());
	if (ft_strncmp(arg, "export", ft_strlen(arg) + 1) == 0)
		return (ft_export(ft_split(str, ' '), shell));
	if (ft_strncmp(arg, "unset", ft_strlen(arg) + 1) == 0)
		return (ft_unset(ft_split(str, ' '), shell));
	if (ft_strncmp(arg, "env", ft_strlen(arg) + 1) == 0)
		return (ft_env(shell));
	if (ft_strncmp(arg, "clear", ft_strlen(arg) + 1) == 0)
		return (system("clear"), 0);
	return (-1);
}

int	is_builtin(char *str)
{
	int		result;
	char	*cmd;

	cmd = simplify(str);
	result = 0;
	if (!cmd)
		return (0);
	if (ft_strncmp(cmd, "echo", ft_strlen(cmd) + 1) == 0)
		result = 1;
	if (ft_strncmp(cmd, "cd", ft_strlen(cmd) + 1) == 0)
		result = 1;
	if (ft_strncmp(cmd, "pwd", ft_strlen(cmd) + 1) == 0)
		result = 1;
	if (ft_strncmp(cmd, "export", ft_strlen(cmd) + 1) == 0)
		result = 1;
	if (ft_strncmp(cmd, "unset", ft_strlen(cmd) + 1) == 0)
		result = 1;
	if (ft_strncmp(cmd, "env", ft_strlen(cmd) + 1) == 0)
		result = 1;
	if (ft_strncmp(cmd, "clear", ft_strlen(cmd) + 1) == 0)
		result = 1;
	return (result);
}


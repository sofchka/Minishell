#include "../minishell.h"

int	ft_pwd(t_shell *sh)
{
	char	*pwd;
	char 	*str;

	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		str = get_env_value("PWD",sh);
		if(str && str[0])
		{
			write(STDOUT_FILENO, str, ft_strlen(str));
			write(STDOUT_FILENO, "\n", 1);
		}
		else
			perror("pwd");
		if (pwd)
			free(pwd);
		if(str)
			free(str);
		return (0);
	}
	write(STDOUT_FILENO, pwd, ft_strlen(pwd));
	write(STDOUT_FILENO, "\n", 1);
	free(pwd);
	return (0);
}

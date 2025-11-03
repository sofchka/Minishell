/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szakarya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 02:33:23 by szakarya          #+#    #+#             */
/*   Updated: 2025/11/04 02:50:57 by szakarya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	cmd_count(char **cmd)
{
	int	i;

	i = 0;
	while (cmd && cmd[i])
		i++;
	return (i);
}

int	ft_cd(t_shell *sh, char **cmd)
{
	char	*path;
	int		ccmd;
	char	*a;
	char	*oldpwd;
	char	**export_dot;
	char	*pwd_value;
	char	*tmp;

	a = NULL;
	ccmd = cmd_count(cmd);
	export_dot = malloc(3 * sizeof(char *));
	export_dot[0] = ft_strdup("export");
	export_dot[1] = NULL;
	export_dot[2] = NULL;
	if (ccmd > 2)
		return (ft_free(export_dot), ft_free(cmd), write(2,
				"bash: cd: too many arguments\n", 29), g_exit_status = 1, 1);
	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
	{
		pwd_value = ft_strjoin("OLDPWD=", get_env_value("PWD", sh), 0);
		export_dot[1] = pwd_value;
		ft_export(sh, export_dot, 0);
	}
	else
	{
		export_dot[1] = ft_strjoin("OLDPWD=", oldpwd, 0);
		ft_export(sh, export_dot, 0);
		free(oldpwd);
	}
	if (ccmd == 1)
	{
		path = get_env_value("HOME", sh);
		if (!path || path[0] == '\0')
			return (ft_free(cmd), write(STDERR_FILENO,
					"bash: cd: HOME not set\n", 23),
				g_exit_status = 1, 1);
		if (chdir(path) == -1)
			return (perror("cd"), ft_free(cmd),
				free(path), g_exit_status = 1, 1);
		free(path);
	}
	else if (ccmd == 2 && !ft_strcmp(cmd[1], "~"))
	{
		if (chdir(sh->home) == -1)
			return (perror("cd"), ft_free(cmd), g_exit_status = 1, 1);
	}
	else if (chdir(cmd[1]) == -1)
	{
		a = ft_strjoin("cd: ", cmd[1], 0);
		if (!a)
			return (ft_free(cmd), g_exit_status = 1, 1);
		ft_free(export_dot);
		perror(a);
		free(a);
		return (ft_free(cmd), g_exit_status = 1, 1);
	}
	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
	{
		tmp = NULL;
		pwd_value = ft_strjoin("PWD=", get_env_value("PWD", sh), 0);
		tmp = pwd_value;
		pwd_value = ft_strjoin(tmp, "/", 0);
		free(tmp);
		tmp = pwd_value;
		pwd_value = ft_strjoin(pwd_value, cmd[1], 0);
		free(tmp);
		export_dot[1] = pwd_value;
		ft_export(sh, export_dot, 0);
		write(1, "cd: error retrieving current directory: getcwd: cannot access parent directories:\n", 82);
	}
	else
	{
		free(export_dot[1]);
		export_dot[1] = ft_strjoin("PWD=", oldpwd, 0);
		ft_export(sh, export_dot, 0);
		free(oldpwd);
	}
	if (g_exit_status == 1)
		return (ft_free(cmd), ft_free(export_dot), 1);
	ft_free(export_dot);
	return (ft_free(cmd), 0);
}

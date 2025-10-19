#include "minishell.h"

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd")
		|| !ft_strcmp(cmd, "pwd") || !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "env")
		|| !ft_strcmp(cmd, "exit"));
}
int	exec_builtin(t_shell *sh, char **cmd, t_exec *cmds)
{
	if (handle_redirection(cmds) == -1)
	{
		restore_std(sh);
		return (1);
	}
	// cd export unset exit aranc fork
	if (!cmd)
		return (1);
	if (!ft_strcmp(cmd[0], "echo"))
		return (ft_echo(sh,cmd));
	else if (!ft_strcmp(cmd[0], "cd"))
		return (ft_cd(sh, cmd));
	else if (!ft_strcmp(cmd[0], "pwd"))
		return (ft_pwd());
	else if (!ft_strcmp(cmd[0], "export"))
		return (ft_export(sh, cmd));
	else if (!ft_strcmp(cmd[0], "unset"))
		return (ft_unset(sh, cmd));
	else if (!ft_strcmp(cmd[0], "env"))
		return (ft_env(sh,cmd));
	else if (!ft_strcmp(cmd[0], "exit"))
		return (ft_exit(cmd, sh), 1);
	restore_std(sh);
	ft_free(cmd);
	return (1);
}

int	ft_pwd(void)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		ft_exit_perror("pwd");
		return (1);
	}
	write(STDOUT_FILENO, pwd, ft_strlen(pwd));
	write(STDOUT_FILENO, "\n", 1);
	free(pwd);
	return (0);
}
//cd

int	cmd_count(char **cmd)
{
	int	i;

	i = 0;
	while (cmd[i] && cmd)
		i++;
	return (i);
}

// char	*env_value(char *name, t_shell *sh)
// {
// 	int		i;
// 	size_t	len;

// 	if (!name || !sh || !sh->env)
// 		return (NULL);
// 	len = ft_strlen(name);
// 	i = 0;
// 	while (sh->env[i])
// 	{
// 		if (!ft_strncmp(sh->env[i], name, len) && sh->env[i][len] == '=')
// 			return (ft_strdup(sh->env[i] + len + 1));
// 		i++;
// 	}
// 	return (NULL);
// }

int	ft_cd(t_shell *sh, char **cmd)
{
	char	*path;
	int		ccmd;

	ccmd = cmd_count(cmd);
	if (ccmd > 2)
		return (write(STDERR_FILENO, "bash: cd: too many arguments\n", 30), g_exit_status = 1, 1);
	if (ccmd == 1)
	{
		path = get_env_value("HOME", sh);
		if (path[0] == '\0')
			return (write(2, "bash: cd: HOME not set\n", 23), g_exit_status = 1, 1);
		if (chdir(path) == -1)
			return (perror("cd"), free(path), g_exit_status = 1, 1);
		free(path);
	}
	else if (ccmd == 2 && !ft_strcmp(cmd[1],"~"))
	{
		if(chdir(sh->home) == -1)
			return (perror("cd"), g_exit_status = 1, 1);
	}
	else
	{
		if (chdir(cmd[1]) == -1)
		{
			char *a = ft_strjoin("cd: ",cmd[1], 0);
			if (!a)
				return(0);
			return (perror(a), free(a), g_exit_status = 1, 1);//?
		}
	}
	return (g_exit_status = 0, 0);
}

// int ft_cd(t_shell *sh, char **cmd)
// {
// 	int cd;
// 	char *path;
// 	//printf("lalalla");s
// 	//printf("%s",cmd[1]);
// 	int ccmd = cmd_count(cmd);
// 	if(ccmd > 2)
// 		return (printf("bash: cd: too many arguments\n"),1);
// 	int i = -1;
// 	while(ccmd == 1 && sh->env[++i])
// 	{
// 		if((path = ft_strnstr(sh->env[i],"HOME=",5)) != NULL)
// 		{
// 			cd = chdir(path + 5);
// 			return cd;
// 		}
// 		//printf("%s\n",sh->env[i]);
// 	}
// 	cd = chdir(cmd[1]);
// 	return 0;
// }


// env
int ft_env(t_shell *shell,char **cmd)
{
	t_env* tmp;

	tmp = shell->t_env;
	if(cmd[1])
	{
		printf("env: '%s': No such file or directory\n", cmd[1]);
		return(g_exit_status = 127, 1);
	}
	while(tmp != NULL)
	{
		if(tmp->key && tmp->value)
		{
			printf("%s=",tmp->key);
			printf("%s\n",tmp->value);
		}
		tmp = tmp->next;
	}
	return 0;
}

//unset

void unset_env(t_shell *sh, char **cmd,int count)
{
	t_env* tmp;
	t_env* prev;
	tmp = sh->t_env;
	prev = NULL;
	while(tmp != NULL)
	{
		if(!ft_strcmp(tmp->key, cmd[count]))
		{
			if (prev == NULL)
				sh->t_env = tmp->next;
			else 
				prev->next = tmp->next;		
			free(tmp->key);
			free(tmp->value);
			free(tmp);
			return ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}


int		ft_unset(t_shell *sh, char **cmd)
{
	if (!sh->t_env)
		return 0;
	int count = cmd_count(cmd);
	while (count-- != 1)
	{
		unset_env(sh,cmd,count);
	}
	return 0;
}

//echo

int is_n_e(char *str)
{
	while(*str && ((*str >= '\t' && *str <= '\r') || *str == ' '))
		str++;
	if (*str != '-')
		return 0;
	str++;
	if(*str == '\0')
		return 0;
	while(*str)
	{
		if(*str != 'n' && *str != 'e')
			return 0;
		str++;
	}
	return 1;
}

int ft_echo(t_shell *sh,char **cmd)
{
	int flag = 0;
	int arg = 1;

	(void)sh;
	while(cmd[arg] && is_n_e(cmd[arg]))
	{
		flag = 1;
		arg++;
	}

	while (cmd[arg]) 
	{
		int i = 0;
		while(cmd[arg][i])
			write(1,&cmd[arg][i++],1);
		arg++;
		if(cmd[arg])
			write(1," ",1);
	}
	if (!flag)
		printf("\n");
	return 0;
}


// export

static void ft_print_export(t_shell *sh)
{
	t_env *node = sh->t_env;
	int count = 0;
	while(node != NULL)
	{
		count++;
		node = node->next;
	}
	if(!count)
		return;
	t_env **arr = malloc(sizeof(t_env *) * count);
	if(!arr)
		return;
	node = sh->t_env;
	int i = 0;
	while(node != NULL)
	{
		arr[i++] = node;
		node = node->next;
	}
	i = 0;
	while(i < count - 1)
	{
		int j = i + 1;
		while(j < count)
		{
			if(strcmp(arr[i]->key,arr[j]->key) > 0)
			{
				t_env *tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
			j++;
		}
		i++;
	}
	i = 0;
	while (i < count)
	{
		if (arr[i]->value)
			printf("declare -x %s=\"%s\"\n", arr[i]->key, arr[i]->value);
		else
			printf("declare -x %s\n", arr[i]->key);
		i++;
	}
	free(arr);
}
int valid_key(char *str)
{
	int i = 0;
	if (!str || (!isalpha(str[0]) && str[0] != '_'))
		return 0;

	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!isalnum(str[i]) && str[i] != '_')
			return 0;
		i++;
	}
	return 1;
}

int export_env(t_shell *sh, char *arg)
{
	if (!valid_key(arg))
	{
		write(2, "bash: export: `", 15);
		write(2, arg, strlen(arg));
		write(2, "': not a valid identifier\n", 26);
		return (0);
	}

	char *eq = ft_strchr(arg, '=');
	char *key;
	char *value = NULL;

	if (eq)
	{
		key = ft_strndup(arg, eq - arg);
		value = ft_strdup(eq + 1);
	}
	else
		key = strdup(arg);

	if (!key || (eq && !value))
	{
		free(key);
		free(value);
		return 0;
	}
	t_env *node = sh->t_env;
	while (node != NULL)
	{
		if (strcmp(node->key, key) == 0)
		{
			if(value != NULL)
			{
				free(node->value);
				node->value = value;
			}
			else
				free(value);
			free(key);
			return 1;
		}
		node = node->next;
	}

	t_env *new_node = malloc(sizeof(t_env));
	if (!new_node)
	{
		free(key);
		free(value);
		return 0;
	}

	new_node->key = key;
	new_node->value = value;
	new_node->next = sh->t_env;
	sh->t_env = new_node;

	return 1;
}

int ft_export(t_shell *sh,char **cmd)
{
	int count = cmd_count(cmd);
	if (count == 1)
		ft_print_export(sh);
	else
	{
		int i = 1;
		while(i < count)
			if(!export_env(sh,cmd[i++]))
				return 1;
	}
	return (g_exit_status = 0,0);
}
#include "../minishell.h"


void	update_old_pwd(char *old_pwd, char **env)
{
	int i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "OLDPWD=", 7) == 0)
			env[i] = ft_strjoin("OLDPWD=", old_pwd);
		i++;
	}
}

void	update_pwd(char *pwd, char **env)
{
	int i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PWD=", 4) == 0)
			env[i] = ft_strjoin("PWD=", pwd);
		i++;
	}
}

char	*ft_chr(char **env, char *vrb)
{
	int i;
	int j;

	i = 0;
	while (env[i])
	{
		if (env[i][0] == *vrb)
		{
			j = 0;
			while (env[i][j] && vrb[j] && env[i][j] == vrb[j])
				j++;
			if (env[i][j] == '=' && !vrb[j])
			{
				return (ft_strdup(*(env + i) + j + 1));
			}
		}
		i++;
	}
	free(vrb);
	return (NULL);
}

void	handle_cd(t_command *cmd, char **env)
{
	char *home;

	if (!ft_strncmp(cmd->args[1], "-", 2))
	{
		chdir(ft_chr(env, "OLDPWD"));
		pwd(cmd, env);
		if (!ft_chr(env, "OLDPWD"))
			ft_putstr_fd("minishell: OLDPWD not set\n", 2);
	}
	else if (cmd->args[1] == NULL || !ft_strncmp(cmd->args[1], "~", 2))
	{
		if (ft_chr(env, "HOME"))
		{
			home = ft_chr(env, "HOME");
			chdir(home);
		}
		else
		{
			ft_putstr_fd("minishell: HOME not set\n", 2);
		}
	}
	else if (chdir(cmd->args[1]))
	{
		perror("minishell");
	}
}

void	cd(t_command *cmd, char **env)
{
	char *old_p;
	char *cwd;

	old_p = getcwd(NULL, 1000);
	update_old_pwd(old_p, env);
	if (cmd->args[2])
	{
		ft_putstr_fd("minishell: too many arguments\n", 2);
		return ;
	}

	handle_cd(cmd, env);
	cwd = getcwd(NULL, 1000);
	update_pwd(cwd, env);
}

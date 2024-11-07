/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_cd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nel-ouar <nel-ouar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 17:05:45 by stakhtou          #+#    #+#             */
/*   Updated: 2024/10/28 21:31:38 by nel-ouar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdbool.h>



void	update_env_variable(char **env,  char *var,  char *value)
{
	char	*new_var;
	int		i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], var, ft_strlen(var)) == 0)
		{
			new_var = ft_strjoin((char *)var, (char *)value);
			env[i] = new_var;
			return ;
		}
		
		i++;
	}
}


void home(char *oldpwd)
{
	char *home_path;

	home_path = getenv("HOME");
	if (home_path)
	{
		chdir(home_path);
		update_env_variable(g_vars.env, "OLDPWD", oldpwd);
		update_env_variable(g_vars.env, "PWD", home_path);
	}
	else
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", 2);
		g_vars.exit_status = 1;
	}
}




void update_current_dir(char *new_path)
{
    if (g_vars.current_dir)
        free(g_vars.current_dir);
    g_vars.current_dir = ft_strdup(new_path);
    update_env_variable(g_vars.env, "PWD=", g_vars.current_dir);
}
void go_oldpwd(void)
{
    if (!g_vars.saved_oldpwd)
    {
        ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
        g_vars.exit_status = 1;
        return;
    }

    if (g_vars.current_dir)
        update_env_variable(g_vars.env, "OLDPWD=", g_vars.current_dir);

    if (chdir(g_vars.saved_oldpwd) == -1)
    {
        ft_putstr_fd("minishell: cd: OLDPWD not accessible\n", 2);
        g_vars.exit_status = 1;
        return;
    }

    update_current_dir(g_vars.saved_oldpwd);
    ft_putstr_fd(g_vars.saved_oldpwd, 1);
    ft_putstr_fd("\n", 1);
}
char *get_current_dir(void)
{
    char *cwd;

    cwd = getcwd(NULL, 0);
    if (cwd)
        return cwd;
    // If getcwd fails, return the tracked path
    if(g_vars.current_dir)
		return ft_strdup(g_vars.current_dir);
	// If the tracked path is not set, try to get it from the environment
	else
	{
		char *pwd = get_env_value("PWD", g_vars.env);
		if (pwd)
			return ft_strdup(pwd);
	}
	return NULL;

}

void init_current_dir(void)
{
    char *cwd;

    cwd = getcwd(NULL, 0);
    if (cwd)
        update_current_dir(cwd);
    else if (!g_vars.current_dir)
    {
        char *pwd = get_env_value("PWD", g_vars.env);
        if (pwd)
            update_current_dir(pwd);
    }
    if (cwd)
        free(cwd);
}

char *build_path(char *base, char *path)
{
    char *result;
    char *temp;

    if (path[0] == '/')
        return ft_strdup(path);
    if (ft_strcmp(path, "..") == 0)
    {
        temp = ft_strjoin(base, "/..");
        return temp;
    }
    result = ft_strjoin(base, "/");
    temp = result;
    result = ft_strjoin(result, path);
    free(temp);

    int i = 0;
    while (result[i])
        i++;
    if (result[i - 1] == '/')
        result[i - 1] = '\0';
    return result;
}



void with_path(char **argv)
{
    char *new_path;

        init_current_dir();

    new_path = build_path(g_vars.current_dir, argv[1]);
    
    if (g_vars.current_dir)
        update_env_variable(g_vars.env, "OLDPWD=", g_vars.current_dir);

    if (chdir(new_path) == -1)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(argv[1], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		g_vars.exit_status = 1;
		free(new_path);
		return;
	}

    update_current_dir(new_path);
    free(new_path);
}
void cd(t_command *cmd)
{
    char *current;

    g_vars.exit_status = 0;
    if (!g_vars.current_dir)
        init_current_dir();
    current = get_current_dir();
   if (current)
    {
		update_env_variable(g_vars.env, "OLDPWD=", current);
		free(current);
	}
    if (cmd->arg_count == 1)
        home(g_vars.saved_oldpwd);
    else if (cmd->arg_count == 2)
    {
        if (ft_strcmp(cmd->args[1], "-") == 0)
            go_oldpwd();
        else
            with_path(cmd->args);
    }
    else
    {
        ft_putstr_fd("minishell: cd: too many arguments\n", 2);
        g_vars.exit_status = 1;
    }
}

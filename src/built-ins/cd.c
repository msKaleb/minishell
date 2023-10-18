/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-m <aperez-m@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/09 09:07:21 by aperez-m          #+#    #+#             */
/*   Updated: 2023/10/13 16:36:57 by aperez-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	free_cd(char *pwd, char *cd_path, char **args, int ret)
{
	free(pwd);
	free(cd_path);
	dbl_free(args);
	return (ret);
}

static int	export_cd_vars(char *cd_path, t_list *lenv, char ***envp_addr)
{
	char	*pwd;
	char	cwd[PATH_MAX];
	char	**args;

	pwd = ft_strdup("export PWD=");
	getcwd(cwd, sizeof(cwd));
	pwd = path_join(pwd, cwd);
	args = ft_split(pwd, ' ');
	export(args, lenv, envp_addr, 1);
	return (free_cd(pwd, cd_path, args, 1));
}

static int	export_oldpwd(t_list *lenv, char ***envp_addr)
{
	char	*oldpwd;
	char	cwd[PATH_MAX];
	char	**args;

	oldpwd = ft_strdup("export OLDPWD=");
	getcwd(cwd, sizeof(cwd));
	oldpwd = path_join(oldpwd, cwd);
	args = ft_split(oldpwd, ' ');
	export(args, lenv, envp_addr, 1);
	return (free_cd(oldpwd, NULL, args, 1));
}

static char	*get_cd_path(char **path, t_list *lenv)
{
	char	*home;
	char	*oldpwd;
	char	*cd_path;

	home = ft_strdup("HOME");
	oldpwd = ft_strdup("OLDPWD");
	if (!path[1])
	{
		cd_path = get_env(lenv, home);
		free(oldpwd);
	}
	else if (ft_strcmp(path[1], "-") == 0)
	{
		cd_path = get_env(lenv, oldpwd);
		free(home);
	}
	else
	{
		cd_path = ft_strdup(path[1]);
		free(home);
		free(oldpwd);
	}
	return (cd_path);
}

int	cd(char **path, t_list *lenv, char ***envp_addr)
{
	char	*cd_path;

	while (*path == NULL)
		path++;
	cd_path = get_cd_path(path, lenv);
	if (!cd_path)
		return (ft_perror(B_GENERIC, path, "env var not defined", 0));
	export_oldpwd(lenv, envp_addr);
	if (chdir(cd_path) == 0)
		return (export_cd_vars(cd_path, lenv, envp_addr));
	ft_perror(B_GENERIC, path, strerror(errno), 1);
	free(cd_path);
	return (errno);
}

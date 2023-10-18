/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_functions.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoria-j <msoria-j@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/11 13:37:04 by msoria-j          #+#    #+#             */
/*   Updated: 2023/10/15 13:39:48 by msoria-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * if err == 0, 'Too many arguments' like error
 * 	'bash: cd: too many arguments'
 * if err == 1, 'No such file or directory' like error
 * 	'bash: cd: non_existent: No such file or directory'
 * if err == 2
 * 	'bash: error_string'
*/
int	ft_perror(int err_num, char **cmd, char *str, int err_mode)
{
	g_error = err_num;
	if (err_mode == 0)
		ft_fprintf(2, MSH_ERR"%s: %s\n", cmd[0], str);
	else if (err_mode == 1)
		ft_fprintf(2, MSH_ERR"%s: %s: %s\n", cmd[0], cmd[1], str);
	else if (err_mode == 2)
		ft_fprintf(2, MSH_ERR"%s\n", str);
	return (1);
}

void	dup_ofile(t_msh **tm)
{
	int	fd;

	if ((*tm)->ofile)
	{
		fd = open((*tm)->ofile, (*tm)->out_mode, 0644);
		if (fd == -1)
		{
			ft_perror(B_GENERIC, &(*tm)->ofile, strerror(errno), 0);
			free((*tm)->ofile);
			(*tm)->ofile = NULL;
			return ;
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
		free((*tm)->ofile);
		(*tm)->ofile = NULL;
	}
}

static char	*get_abs_path(char *cmd, t_msh *tm)
{
	char	*tmp_path;
	char	*path_cmd;
	int		i;

	if (!tm->paths)
		return (NULL);
	if (is_built_in(cmd))
		return (NULL);
	if (access(cmd, F_OK | X_OK) == 0)
		return (ft_strdup(cmd));
	i = -1;
	while (tm->paths[++i])
	{
		tmp_path = ft_strjoin(tm->paths[i], "/");
		path_cmd = ft_strjoin(tmp_path, cmd);
		free(tmp_path);
		if (access(path_cmd, F_OK | X_OK) == 0)
			return (path_cmd);
		free(path_cmd);
	}
	return (NULL);
}

void	export_last_command(char *cmd, t_msh *tm)
{
	char	**args;
	char	*path;
	char	*cmd_wpath;

	path = ft_strdup("export _=");
	args = ft_split(cmd, 20);
	cmd_wpath = get_abs_path(args[0], tm);
	if (cmd_wpath)
		path = path_join(path, cmd_wpath);
	else
	{
		path = path_join(path, "built-in/");
		path = path_join(path, args[0]);
	}
	free(cmd_wpath);
	dbl_free(args);
	args = ft_split(path, ' ');
	export(args, *(tm->tenvp), &tm->envp, 1);
	free(path);
	dbl_free(args);
}

int	pipe_and_fork(t_process *proc)
{
	if (pipe(proc->pipe_fd) < 0)
	{
		perror("pipe");
		return (-1);
	}
	proc->fork_id = fork();
	if (proc->fork_id < 0)
	{
		perror("fork");
		return (-1);
	}
	return (0);
}

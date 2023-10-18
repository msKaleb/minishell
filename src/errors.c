/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoria-j <msoria-j@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 10:10:45 by msoria-j          #+#    #+#             */
/*   Updated: 2023/10/17 16:52:14 by msoria-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	input_error(t_msh *tm, char **ofile)
{
	if (*ofile)
		free(*ofile);
	free(tm->ofile);
	tm->ofile = NULL;
	*ofile = NULL;
	return (-1);
}

char	*cmd_exist(char **cmd, t_msh *tm)
{
	char	*tmp_path;
	char	*path_cmd;
	int		i;

	if (!tm->paths)
		return (NULL);
	if (access(cmd[0], F_OK) == 0)
		return (cmd[0]);
	i = -1;
	while (tm->paths[++i])
	{
		tmp_path = ft_strjoin(tm->paths[i], "/");
		path_cmd = ft_strjoin(tmp_path, cmd[0]);
		free(tmp_path);
		if (access(path_cmd, F_OK) == 0)
			return (path_cmd);
		free(path_cmd);
	}
	return (NULL);
}

void	cmd_not_found(t_msh *tm, char **args)
{
	char	*cmd;

	cmd = cmd_exist(args, tm);
	if (cmd)
		ft_perror(B_NOT_PERM, args, "permission denied", 0);
	else
		ft_perror(B_NOT_FOUND, args, "command not found", 0);
	dbl_free(args);
	ft_exit(tm, g_error);
}

int	check_exit_arg(char *arg, char **cmd)
{
	int	i;

	i = -1;
	if (!cmd[1])
		return (1);
	while (arg[++i])
	{
		if (arg[i] < '0' || arg[i] > '9')
		{
			ft_perror(B_GENERIC, cmd, "numeric argument required", 1);
			return (0);
		}
	}
	return (1);
}

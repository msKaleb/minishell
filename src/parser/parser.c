/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-m <aperez-m@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 13:28:42 by msoria-j          #+#    #+#             */
/*   Updated: 2023/10/13 16:46:11 by aperez-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Change argument when it's . or .. to its respective value
*/
static char	*parse_args(char *cmd, char *arg)
{
	char	*cdir;
	char	cwd[PATH_MAX];
	char	*path;

	if (ft_strcmp(cmd, "echo") == 0)
		return (ft_strdup(arg));
	getcwd(cwd, sizeof(cwd));
	path = ft_strdup(cwd);
	if (ft_strcmp(arg, ".") == 0)
		return (path);
	else if (ft_strcmp(arg, "..") == 0)
	{
		cdir = ft_strrchr(path, '/');
		if (ft_strcmp(cdir, path) == 0)
			cdir = path + 1;
		*cdir = '\0';
		return (path);
	}
	free(path);
	return (ft_strdup(arg));
}

static void	exec_child_and_free(char **command, t_msh *tm, char **ofile)
{
	exec_child(*command, tm);
	free(*ofile);
	*ofile = NULL;
	free(*command);
	*command = ft_strdup("");
}

static void	fill_command(char **command, char *token, char *cmd, int is_quoted)
{
	char	*arg;
	char	*twenty;

	twenty = ft_calloc(2, 1);
	twenty[0] = 20;
	arg = parse_args(cmd, token);
	if (is_quoted == 1
		|| (!is_redirector(token) && ft_strcmp(token, "|") != 0))
		*command = path_join(*command, arg);
	*command = path_join(*command, twenty);
	free(twenty);
	free(arg);
}

static void	exec_parent_and_free(char **command, t_msh *tm)
{
	free(tm->is_quoted);
	tm->is_quoted = NULL;
	if (ft_strlen(*command) > 0 && !is_empty_cmd(*command))
		exec_parent(*command, tm);
	free(*command);
	restore_io(tm->fdin, tm->fdout);
}

void	get_command_table(t_msh *tm)
{
	char	*command;
	int		i;

	i = -1;
	command = ft_strdup("");
	set_signal_action(ON_BACKGROUND);
	if (check_inputs(tm) == -1)
	{
		free(command);
		return ;
	}
	while (tm->l_exp[++i])
	{
		if (op_selector(tm, i, &tm->ofile, 0) == 1)
			i += 2;
		if (!tm->l_exp[i])
			break ;
		fill_command(&command, tm->l_exp[i], tm->l_exp[0], tm->is_quoted[i]);
		if (is_pipe(tm, i, command))
			exec_child_and_free(&command, tm, &tm->ofile);
		if (is_redirector(tm->l_exp[i]) && tm->is_quoted[i] == 0)
			i--;
	}
	exec_parent_and_free(&command, tm);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoria-j <msoria-j@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 16:06:08 by msoria-j          #+#    #+#             */
/*   Updated: 2023/10/11 17:23:06 by msoria-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

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
	tm->and_or = MSH_PIPE;
}

static void	fill_command(char **command, char *token, char *cmd, int is_quoted)
{
	char	*arg;
	char	*twenty;

	twenty = ft_calloc(2, 1);
	twenty[0] = 20;
	arg = parse_args(cmd, token);
	if (is_quoted == 1
		|| (!is_redirector(token) && !is_c_operator(token, *command)))
		*command = path_join(*command, arg);
	*command = path_join(*command, twenty);
	free(twenty);
	free(arg);
}

static void	exec_parent_and_free(char **command, t_msh *tm)
{
	free(tm->is_quoted);
	tm->is_quoted = NULL;
	if (!successful_cmd(tm))
	{
		free(*command);
		tm->and_or = MSH_PIPE;
		return ;
	}
	if (ft_strlen(*command) > 0 && !is_empty_cmd(*command))
		exec_parent(*command, tm);
	free(*command);
	restore_io(tm->fdin, tm->fdout);
	tm->and_or = MSH_PIPE;
}

void	get_command_table(t_msh *tm)
{
	char	*command;
	int		i;

	i = -1;
	command = ft_strdup("");
	set_signal_action(ON_BACKGROUND);
	if (check_inputs(tm) == -1)
		return (free(command));
	while (tm->l_exp[++i])
	{
		if (is_parenthesis(tm->l_exp[i]) && !tm->is_quoted[i])
			continue ;
		if (op_selector(tm, i, &tm->ofile, 0) == 1)
			i += 2;
		if (!tm->l_exp[i])
			break ;
		fill_command(&command, tm->l_exp[i], tm->l_exp[0], tm->is_quoted[i]);
		if (is_pipe(tm, i, command))
			exec_child_and_free(&command, tm, &tm->ofile);
		else if (is_c_operator(tm->l_exp[i], command) && tm->is_quoted[i] == 0)
			exec_and_or(&command, tm, &i);
		if (is_redirector(tm->l_exp[i]) && tm->is_quoted[i] == 0)
			i--;
	}
	exec_parent_and_free(&command, tm);
}

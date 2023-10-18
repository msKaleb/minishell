/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_functions_2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoria-j <msoria-j@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 17:26:03 by msoria-j          #+#    #+#             */
/*   Updated: 2023/10/11 10:00:37 by msoria-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_hd(t_msh *tm, int index)
{
	restore_io(tm->fdin, tm->fdout);
	tm->fdin = dup(STDIN_FILENO);
	tm->fdout = dup(STDOUT_FILENO);
	here_doc(tm->l_exp[index + 1]);
}

/**
 * @brief In case of redirector, opens the file for IO
* have to create the file even if we don't write to it
*/
int	op_selector(t_msh *tm, int index, char **ofile, int no_hd)
{
	if (tm->is_quoted[index] == 1)
		return (0);
	if (ft_strcmp(tm->l_exp[index], "<") == 0)
	{
		if (create_input(tm->l_exp[index + 1], R_INPUT, STDIN_FILENO) == -1)
		{
			restore_io(tm->fdin, tm->fdout);
			return (-1);
		}
		return (1);
	}
	else if (ft_strcmp(tm->l_exp[index], ">") == 0)
		return (create_output(tm, tm->l_exp[index + 1], R_OUTPUT, ofile));
	else if (ft_strcmp(tm->l_exp[index], "<<") == 0)
	{
		if (no_hd == 0)
			exec_hd(tm, index);
		return (1);
	}
	else if (ft_strcmp(tm->l_exp[index], ">>") == 0)
		return (create_output(tm, tm->l_exp[index + 1], R_APPEND, ofile));
	return (0);
}

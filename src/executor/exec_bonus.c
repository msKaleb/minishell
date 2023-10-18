/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoria-j <msoria-j@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 16:01:02 by msoria-j          #+#    #+#             */
/*   Updated: 2023/10/05 20:08:57 by msoria-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

void	exec_selector(t_msh *tm, char **command, int i)
{
	if (ft_strcmp(tm->l_exp[i], "|") == 0 && ft_strcmp(*command, "\024") != 0)
	{
		exec_child(*command, tm);
		free(tm->ofile);
		tm->ofile = NULL;
		free(*command);
		*command = ft_strdup("");
	}
}

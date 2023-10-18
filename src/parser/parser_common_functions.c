/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_common_functions.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoria-j <msoria-j@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 13:10:24 by msoria-j          #+#    #+#             */
/*   Updated: 2023/10/11 17:26:30 by msoria-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_pipe(t_msh *tm, int index, char *command)
{
	if (tm->is_quoted[index] == 0
		&& ft_strcmp(tm->l_exp[index], "|") == 0
		&& ft_strcmp(command, "\024") != 0)
		return (1);
	return (0);
}

int	is_empty_cmd(char *cmd)
{
	while (*cmd)
	{
		if (*cmd != '\024')
			return (0);
		cmd++;
	}
	return (1);
}

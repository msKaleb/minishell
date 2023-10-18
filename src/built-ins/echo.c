/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoria-j <msoria-j@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/09 09:07:25 by aperez-m          #+#    #+#             */
/*   Updated: 2023/08/22 11:00:27 by msoria-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	echo(char **cmd)
{
	int	i;
	int	j;

	i = 1;
	if (!cmd[1])
	{
		write(1, "\n", 1);
		return (1);
	}
	else if (ft_strcmp(cmd[1], "-n") == 0)
		i++;
	while (cmd[i])
	{
		j = -1;
		while (cmd[i][++j])
			write(1, &cmd[i][j], 1);
		i++;
		if (cmd[i])
			write(1, " ", 1);
	}
	if (ft_strcmp(cmd[1], "-n") != 0)
		write(1, "\n", 1);
	return (1);
}

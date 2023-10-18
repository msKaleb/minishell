/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoria-j <msoria-j@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/09 09:09:00 by aperez-m          #+#    #+#             */
/*   Updated: 2023/10/14 11:59:36 by msoria-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_exit(t_msh *tm, int wstatus)
{
	char	status;

	status = wstatus;
	ft_lstclear(tm->tenvp, &free);
	dbl_free(tm->line_read_split);
	dbl_free(tm->l_exp);
	dbl_free(tm->paths);
	dbl_free(tm->envp);
	free(tm->line_read);
	free(tm->ofile);
	exit(status);
}

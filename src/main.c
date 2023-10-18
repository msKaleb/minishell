/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoria-j <msoria-j@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/09 09:13:49 by aperez-m          #+#    #+#             */
/*   Updated: 2023/10/06 14:19:18 by msoria-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

sig_atomic_t	g_error;

int	main(int argc, char **argv, char **penv)
{
	t_msh	*tm;

	(void)argc;
	(void)argv;
	tm = malloc(sizeof(*tm));
	tm->line_read_split = NULL;
	tm->l_exp = NULL;
	tm->is_quoted = NULL;
	tm->ofile = NULL;
	tm->envp = NULL;
	tm->tenvp = fill_penv_list(penv);
	tm->envp = fill_penv_dptr(tm->tenvp);
	tm->pids = ft_calloc(1, sizeof(**tm->pids));
	while (1)
	{
		tm->fdin = dup(STDIN_FILENO);
		tm->fdout = dup(STDOUT_FILENO);
		minishell(tm);
	}
}

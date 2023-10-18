/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoria-j <msoria-j@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/26 20:26:41 by aperez-m          #+#    #+#             */
/*   Updated: 2023/10/05 12:49:20 by msoria-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	minishell(t_msh *tm)
{
	int		i;
	char	*temp;
	char	*temp2;

	temp2 = ft_strdup("PATH");
	temp = get_env(*tm->tenvp, temp2);
	tm->paths = ft_split(temp, ':');
	free(temp);
	tm->line_read = ft_readline(tm);
	lexer(&tm->line_read);
	if (tm->line_read)
	{
		tm->line_read_split = ft_split_msh(tm->line_read, ' ');
		tm->l_exp = line_expander(&tm->line_read_split, *tm->tenvp);
		i = -1;
		is_quoted_array(tm);
		while (tm->l_exp[++i])
			quote_remove(tm->l_exp[i]);
		if (tm->l_exp[0] && tm->l_exp[0][0])
			get_command_table(tm);
		free_on_loop(tm);
	}
	dbl_free(tm->paths);
}
